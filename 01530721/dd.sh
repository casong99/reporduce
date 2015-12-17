#!/bin/bash

dd if=/dev/zero ibs=1M count=100 | tr "\000" "\377" >7M.test
