# AverageTagsQuals Unification Migration Guide

## Summary

The 48+ length-specific `AverageTagsQuals_*.cc` files have been unified into a single `AverageTagsQuals.cc` program that accepts tag length as a command-line parameter.

## What Changed

### Before
```bash
# Old approach: Separate executables for each tag length
./AverageTagsQuals_27 input.txt
./AverageTagsQuals_36 input.txt
./AverageTagsQuals_50 input.txt
# ... 48+ different executables
```

### After
```bash
# New approach: Single executable with tag length parameter
./AverageTagsQuals input.txt 27
./AverageTagsQuals input.txt 36
./AverageTagsQuals input.txt 50
```

## Migration Instructions

### For Users

Replace old command patterns with the new syntax:

**Old:**
```bash
AverageTagsQuals_<LENGTH> <input_file>
```

**New:**
```bash
AverageTagsQuals <input_file> <LENGTH>
```

**Examples:**
- `AverageTagsQuals_27 data.txt` → `AverageTagsQuals data.txt 27`
- `AverageTagsQuals_100 reads.txt` → `AverageTagsQuals reads.txt 100`

### For Developers

The unified implementation is in `src/AverageTagsQuals.cc`:
- Accepts two arguments: input file and expected tag length
- Validates tag length is a positive integer
- Provides better error messages with expected vs actual lengths
- Identical algorithm to the original versions

### Archived Files

All 48 length-specific source files have been moved to `src/legacy/`:
- `AverageTagsQuals_17.cc` through `AverageTagsQuals_104.cc`
- Preserved for reference but no longer compiled
- Tag probability variants (`*_tag_prb.cc`) remain active

## Benefits

1. **Maintainability**: One codebase instead of 48+ nearly identical files
2. **Flexibility**: Support any tag length without creating new files
3. **Reduced complexity**: Simplified Makefile and build process
4. **Lower maintenance burden**: Bug fixes apply to all tag lengths automatically
5. **Reduced binary size**: One executable instead of 48+

## Testing

The unified version has been tested and produces identical results to the original length-specific versions.

```bash
# Build the unified version
cd src && make AverageTagsQuals

# Test with different tag lengths
./AverageTagsQuals test_data_27.txt 27
./AverageTagsQuals test_data_36.txt 36
```

## Backward Compatibility

If needed, create wrapper scripts for backward compatibility:

```bash
#!/bin/bash
# AverageTagsQuals_27 wrapper
exec ./AverageTagsQuals "$1" 27
```

## Questions?

Contact the maintainer or refer to `src/AverageTagsQuals.cc` for implementation details.
