#!/bin/bash
# no-sigterm.sh
trap "" SIGTERM 
exec "$@"