CXX = g++ -O3 -march=native -flto -Wall

all: GenerateProportion EstimateTrueCount FindNeighboursWithQual \
	AverageTagsQuals AverageTagsQuals_tag_prb PickBaseQual \
	AverageTagsQuals_27_tag_prb AverageTagsQuals_36_tag_prb


FindNeighboursWithQual: FindNeighboursWithQual.cc Utilities.cc
	$(CXX)   $^ -o $@

GenerateProportion: GenerateProportion.cc Utilities.cc
	$(CXX)   $^ -o $@

# Optimized unified versions (Phase 3 optimization)
AverageTagsQuals: AverageTagsQuals.cc
	$(CXX)   $^ -o $@

AverageTagsQuals_tag_prb: AverageTagsQuals_tag_prb.cc
	$(CXX)   $^ -o $@

PickBaseQual: PickBaseQual.cc
	$(CXX)   $^ -o $@

AverageTagsQuals_27_tag_prb: AverageTagsQuals_27_tag_prb.cc
	$(CXX)   $^ -o $@
AverageTagsQuals_36_tag_prb: AverageTagsQuals_36_tag_prb.cc
	$(CXX)   $^ -o $@

EstimateTrueCount: EstimateTrueCount.cc Utilities.cc
	$(CXX)   $^ -o $@

