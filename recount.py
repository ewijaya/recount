#!/usr/bin/env python3
"""
RECOUNT Wrapper - Python Implementation

A modern, robust wrapper for the RECOUNT probabilistic error correction pipeline.
Provides improved error handling, logging, and usability over the original Perl wrapper.

Usage:
    python recount.py <input_file> <num_mismatches> [options]

Example:
    python recount.py test-data.txt 1 --verbose
    python recount.py data.txt 2 --keep-temp --min-base-error 0.001
"""

import argparse
import logging
import os
import sys
import subprocess
import shutil
from pathlib import Path
from datetime import datetime


class RecountPipeline:
    """Manages the RECOUNT error correction pipeline execution."""

    # Exit codes
    EXIT_SUCCESS = 0
    EXIT_INVALID_ARGS = 1
    EXIT_FILE_NOT_FOUND = 2
    EXIT_BINARY_NOT_FOUND = 3
    EXIT_EXECUTION_ERROR = 4
    EXIT_INVALID_FORMAT = 5

    def __init__(self, input_file, num_mismatches, min_base_error=0.00262689,
                 src_path="./src", keep_temp=False, verbose=False,
                 output_dir=None, dry_run=False):
        """
        Initialize the RECOUNT pipeline.

        Args:
            input_file: Path to the preprocessed input file
            num_mismatches: Number of mismatches to neighbors (1 or 2)
            min_base_error: Minimum base error probability (default: 0.00262689)
            src_path: Path to the directory containing binaries
            keep_temp: Keep temporary files after completion
            verbose: Enable verbose logging
            output_dir: Optional output directory (default: same as input)
            dry_run: Show what would be executed without running
        """
        self.input_file = Path(input_file).resolve()
        self.num_mismatches = num_mismatches
        self.min_base_error = min_base_error
        self.src_path = Path(src_path).resolve()
        self.keep_temp = keep_temp
        self.verbose = verbose
        self.output_dir = Path(output_dir) if output_dir else self.input_file.parent
        self.dry_run = dry_run

        # Setup logging
        self._setup_logging()

        # File paths
        self.base_name = self.input_file.stem
        self.nb_file = self.output_dir / f"{self.base_name}.nb"
        self.prop_file = self.output_dir / f"{self.base_name}.prop"
        self.nbq_file = self.output_dir / f"{self.base_name}.nbq"

        # Binary paths
        self.find_neighbours = self.src_path / "FindNeighboursWithQual"
        self.generate_proportion = self.src_path / "GenerateProportion"
        self.estimate_true_count = self.src_path / "EstimateTrueCount"

        self.logger.info(f"RECOUNT Pipeline initialized")
        self.logger.info(f"Input file: {self.input_file}")
        self.logger.info(f"Number of mismatches: {self.num_mismatches}")
        self.logger.info(f"Minimum base error: {self.min_base_error}")

    def _setup_logging(self):
        """Configure logging based on verbosity level."""
        log_level = logging.DEBUG if self.verbose else logging.INFO
        log_format = '%(asctime)s - %(levelname)s - %(message)s'

        # Configure root logger
        logging.basicConfig(
            level=log_level,
            format=log_format,
            handlers=[
                logging.StreamHandler(sys.stderr)
            ]
        )
        self.logger = logging.getLogger(__name__)

    def validate_inputs(self):
        """
        Validate all input parameters and file existence.

        Returns:
            True if all validations pass

        Raises:
            SystemExit on validation failure
        """
        self.logger.info("Validating inputs...")

        # Check input file existence
        if not self.input_file.exists():
            self.logger.error(f"Input file not found: {self.input_file}")
            sys.exit(self.EXIT_FILE_NOT_FOUND)

        if not self.input_file.is_file():
            self.logger.error(f"Input path is not a file: {self.input_file}")
            sys.exit(self.EXIT_FILE_NOT_FOUND)

        # Check if input file is readable
        if not os.access(self.input_file, os.R_OK):
            self.logger.error(f"Input file is not readable: {self.input_file}")
            sys.exit(self.EXIT_FILE_NOT_FOUND)

        # Validate number of mismatches
        if self.num_mismatches not in [1, 2]:
            self.logger.error(f"Number of mismatches must be 1 or 2, got: {self.num_mismatches}")
            sys.exit(self.EXIT_INVALID_ARGS)

        # Validate min base error
        if not (0 < self.min_base_error < 1):
            self.logger.error(f"Minimum base error must be between 0 and 1, got: {self.min_base_error}")
            sys.exit(self.EXIT_INVALID_ARGS)

        # Check output directory
        if not self.output_dir.exists():
            self.logger.warning(f"Output directory does not exist, creating: {self.output_dir}")
            try:
                self.output_dir.mkdir(parents=True, exist_ok=True)
            except Exception as e:
                self.logger.error(f"Failed to create output directory: {e}")
                sys.exit(self.EXIT_EXECUTION_ERROR)

        # Validate input file format (basic check)
        try:
            with open(self.input_file, 'r') as f:
                first_line = f.readline().strip()
                if first_line:
                    parts = first_line.split()
                    if len(parts) < 3:
                        self.logger.error(
                            f"Invalid input format. Expected: <count> <sequence> <qual1> [qual2] ... "
                            f"Got {len(parts)} columns"
                        )
                        sys.exit(self.EXIT_INVALID_FORMAT)
                    # Check if first column is a number
                    try:
                        int(parts[0])
                    except ValueError:
                        self.logger.error(f"First column should be a count (integer), got: {parts[0]}")
                        sys.exit(self.EXIT_INVALID_FORMAT)
                else:
                    self.logger.error("Input file is empty")
                    sys.exit(self.EXIT_INVALID_FORMAT)
        except Exception as e:
            self.logger.error(f"Error reading input file: {e}")
            sys.exit(self.EXIT_FILE_NOT_FOUND)

        self.logger.info("Input validation passed")
        return True

    def check_binaries(self):
        """
        Check if all required binaries exist and are executable.

        Returns:
            True if all binaries are found and executable

        Raises:
            SystemExit if any binary is missing
        """
        self.logger.info("Checking for required binaries...")

        binaries = [
            ("FindNeighboursWithQual", self.find_neighbours),
            ("GenerateProportion", self.generate_proportion),
            ("EstimateTrueCount", self.estimate_true_count)
        ]

        for name, path in binaries:
            if not path.exists():
                self.logger.error(f"Binary not found: {name} at {path}")
                self.logger.error(f"Please compile binaries by running 'make' in the src/ directory")
                sys.exit(self.EXIT_BINARY_NOT_FOUND)

            if not os.access(path, os.X_OK):
                self.logger.error(f"Binary is not executable: {name} at {path}")
                self.logger.error(f"Try running: chmod +x {path}")
                sys.exit(self.EXIT_BINARY_NOT_FOUND)

            self.logger.debug(f"Found binary: {name} at {path}")

        self.logger.info("All required binaries found")
        return True

    def _run_command(self, cmd, description, capture_output=False):
        """
        Execute a command with error handling.

        Args:
            cmd: Command to execute (list or string)
            description: Human-readable description of the command
            capture_output: If True, capture stdout (default: False)

        Returns:
            CompletedProcess object if successful

        Raises:
            SystemExit on command failure
        """
        self.logger.info(f"{description}...")
        self.logger.debug(f"Executing: {' '.join(str(c) for c in cmd)}")

        if self.dry_run:
            self.logger.info(f"[DRY RUN] Would execute: {' '.join(str(c) for c in cmd)}")
            return None

        try:
            if capture_output:
                result = subprocess.run(
                    cmd,
                    check=True,
                    capture_output=True,
                    text=True
                )
                self.logger.debug(f"Command completed successfully")
                return result
            else:
                result = subprocess.run(
                    cmd,
                    check=True
                )
                self.logger.info(f"{description} completed successfully")
                return result

        except subprocess.CalledProcessError as e:
            self.logger.error(f"{description} failed with exit code {e.returncode}")
            if e.stderr:
                self.logger.error(f"Error output: {e.stderr}")
            sys.exit(self.EXIT_EXECUTION_ERROR)
        except Exception as e:
            self.logger.error(f"Unexpected error during {description}: {e}")
            sys.exit(self.EXIT_EXECUTION_ERROR)

    def run_pipeline(self):
        """Execute the complete RECOUNT pipeline."""
        start_time = datetime.now()
        self.logger.info("="*60)
        self.logger.info("Starting RECOUNT pipeline")
        self.logger.info("="*60)

        # Step 1: Find Neighbours With Quality
        cmd = [
            str(self.find_neighbours),
            str(self.input_file),
            str(self.num_mismatches),
            str(self.min_base_error)
        ]
        self._run_command(cmd, "FindNeighboursWithQual")

        # Step 2: Generate Proportion
        cmd = [str(self.generate_proportion), str(self.input_file)]
        result = self._run_command(cmd, "GenerateProportion", capture_output=True)

        if not self.dry_run:
            try:
                with open(self.prop_file, 'w') as f:
                    f.write(result.stdout)
                self.logger.debug(f"Wrote proportion data to {self.prop_file}")
            except Exception as e:
                self.logger.error(f"Failed to write proportion file: {e}")
                sys.exit(self.EXIT_EXECUTION_ERROR)

        # Step 3: Estimate True Count
        cmd = [str(self.estimate_true_count), str(self.input_file)]
        self._run_command(cmd, "EstimateTrueCount")

        # Cleanup temporary files
        if not self.keep_temp and not self.dry_run:
            self._cleanup_temp_files()

        end_time = datetime.now()
        duration = (end_time - start_time).total_seconds()

        self.logger.info("="*60)
        self.logger.info(f"RECOUNT pipeline completed successfully")
        self.logger.info(f"Total execution time: {duration:.2f} seconds")
        self.logger.info("="*60)

        # Show output files
        output_file = self.output_dir / f"{self.base_name}.recount"
        if output_file.exists():
            self.logger.info(f"Output file: {output_file}")

    def _cleanup_temp_files(self):
        """Remove temporary files created during pipeline execution."""
        temp_files = [self.nb_file, self.prop_file, self.nbq_file]

        self.logger.info("Cleaning up temporary files...")
        for temp_file in temp_files:
            if temp_file.exists():
                try:
                    temp_file.unlink()
                    self.logger.debug(f"Removed: {temp_file}")
                except Exception as e:
                    self.logger.warning(f"Failed to remove {temp_file}: {e}")

    def run(self):
        """Main execution method - validates inputs and runs pipeline."""
        try:
            self.validate_inputs()
            self.check_binaries()
            self.run_pipeline()
            return self.EXIT_SUCCESS
        except KeyboardInterrupt:
            self.logger.warning("\nPipeline interrupted by user")
            return 130  # Standard exit code for SIGINT
        except Exception as e:
            self.logger.error(f"Unexpected error: {e}")
            if self.verbose:
                import traceback
                traceback.print_exc()
            return self.EXIT_EXECUTION_ERROR


def parse_arguments():
    """Parse and validate command-line arguments."""
    parser = argparse.ArgumentParser(
        description="RECOUNT: Probabilistic Error Correction for Next Generation Sequencing Data",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s test-data.txt 1
  %(prog)s data.txt 2 --verbose --keep-temp
  %(prog)s input.txt 1 --min-base-error 0.001 --output-dir ./results
  %(prog)s input.txt 2 --dry-run

For more information, see README.md or visit:
http://www.ncbi.nlm.nih.gov/pubmed/20180274
        """
    )

    # Required arguments
    parser.add_argument(
        'input_file',
        help='Preprocessed input file (format: <count> <sequence> <qual1> [qual2] ...)'
    )

    parser.add_argument(
        'num_mismatches',
        type=int,
        choices=[1, 2],
        help='Number of mismatches to neighbors (1 or 2)'
    )

    # Optional arguments
    parser.add_argument(
        '--min-base-error',
        type=float,
        default=0.00262689,
        metavar='FLOAT',
        help='Minimum base error probability (default: 0.00262689, affects mismatch=2 only)'
    )

    parser.add_argument(
        '--src-path',
        default='./src',
        metavar='PATH',
        help='Path to directory containing RECOUNT binaries (default: ./src)'
    )

    parser.add_argument(
        '--output-dir',
        metavar='PATH',
        help='Output directory for results (default: same as input file directory)'
    )

    parser.add_argument(
        '--keep-temp',
        action='store_true',
        help='Keep temporary files (.nb, .prop, .nbq) after completion'
    )

    parser.add_argument(
        '--verbose', '-v',
        action='store_true',
        help='Enable verbose output for debugging'
    )

    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Show what would be executed without actually running the pipeline'
    )

    parser.add_argument(
        '--version',
        action='version',
        version='RECOUNT Pipeline Wrapper v2.0'
    )

    args = parser.parse_args()

    return args


def main():
    """Main entry point for the RECOUNT wrapper."""
    args = parse_arguments()

    # Create and run pipeline
    pipeline = RecountPipeline(
        input_file=args.input_file,
        num_mismatches=args.num_mismatches,
        min_base_error=args.min_base_error,
        src_path=args.src_path,
        keep_temp=args.keep_temp,
        verbose=args.verbose,
        output_dir=args.output_dir,
        dry_run=args.dry_run
    )

    exit_code = pipeline.run()
    sys.exit(exit_code)


if __name__ == '__main__':
    main()
