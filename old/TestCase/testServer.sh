#!/bin/bash
ab -n $1 -c $2 http://127.0.0.1:8888/index.html
