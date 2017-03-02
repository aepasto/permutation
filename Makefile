DEBUG=-g
PRODUCTION=-O3
CPP=g++
CFLAGS=-Wall -fmessage-length=0  -std=c++0x  -Wextra -fpermissive $(PRODUCTION)
LDFLAGS=

# SOURCES.
SOURCES=KMeansRank.cpp Graph.cpp GraphUtils.cpp PowerIterationPPR.cpp RankAggregationAvg.cpp RandomVisit.cpp Ranking.cpp RankAggregation.cpp RankDistance.cpp
BINARY_SOURCES=OutputRankings.cpp RunKMeans.cpp RunDistanceStat.cpp RunReconstructBeginTest.cpp

# OBJECTS.
OBJECTS=$(SOURCES:.cpp=.o)
BINARY_OBJECTS=$(BINARY_SOURCES:.cpp=.o)
ALL_LOCAL_OBJECTS=$(OBJECTS) $(BINARY_OBJECTS)

# DEPENDENCIES.
DEPS=$(patsubst %.o,%.d,$(ALL_LOCAL_OBJECTS))

# BINARIES.
BINARIES=$(BINARY_SOURCES:.cpp=)

# RULES.

all: $(BINARIES) $(OBJECTS)

$(ALL_LOCAL_OBJECTS): %.o: %.cpp
	$(CPP) -MMD -MP $(CFLAGS) -c $< -o $@
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

$(BINARIES): %: %.o $(OBJECTS)
	$(CPP) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(DEPS) $(ALL_LOCAL_OBJECTS) $(BINARIES) *.d-e *.d

-include $(DEPS)
