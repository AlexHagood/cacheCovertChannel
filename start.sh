#!/bin/bash

terminal="kitty" #Change this to your terminal emulator! I use kitty, but yours could be gnome-terminal or something else.

echo "Starting receiver"
"$terminal" -- ./receiver

echo "Starting sender"
"$terminal" -- ./receiver