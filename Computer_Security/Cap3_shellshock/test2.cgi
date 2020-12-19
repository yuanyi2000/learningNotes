#!/bin/bash_shellshock

echo "Content-type: text/plain"
echo
echo "** Enviroment Variables **"
strings /proc/$$/environ