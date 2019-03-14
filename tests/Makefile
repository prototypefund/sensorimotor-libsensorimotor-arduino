.PHONY: test
test: conversion.bin pid.bin clean success

.PHONY: clean
clean:
	rm *.bin

%.bin: %.cpp
	g++ -std=gnu++11 -o $@ $<
	./$@

.PHONY: success
success:
	echo "All tests were executed successfully!"