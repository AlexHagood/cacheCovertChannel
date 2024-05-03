CXXFLAGS = -Wall -Wextra -std=c++11
DEBUG_CXXFLAGS = -g

RECEIVER_EXEC = receiver
SENDER_EXEC = sender

RECEIVER_SRCS = util.cpp receiver.cpp
SENDER_SRCS = util.cpp sender.cpp

RECEIVER_OBJS = $(RECEIVER_SRCS:.cpp=.o)
SENDER_OBJS = $(SENDER_SRCS:.cpp=.o)

HDRS = util.hpp

.PHONY: all clean debug

all: $(RECEIVER_EXEC) $(SENDER_EXEC)

$(RECEIVER_EXEC): $(RECEIVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SENDER_EXEC): $(SENDER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $<

debug: CXXFLAGS += $(DEBUG_CXXFLAGS)
debug: all

clean:
	rm -f $(RECEIVER_EXEC) $(RECEIVER_OBJS) $(SENDER_EXEC) $(SENDER_OBJS)
