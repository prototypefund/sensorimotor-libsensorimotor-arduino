.PHONY: test
test: conversion.bin pid.bin clean

.PHONY: clean
clean:
	rm *.bin

%.bin: %.cpp
	g++ -o $@ $<
	./$@