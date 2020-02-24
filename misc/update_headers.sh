#!/bin/bash

if [ ! -f "misc/header_cpp.txt" ] || [ ! -f "misc/header_lua.txt" ];
then
	echo "Failed to find header files. Aborting."
	exit
fi

for file in $(find client server common -name "*.*pp");
do
	sed -i '1,26d' $file
	cat misc/header_cpp.txt $file > $file.new && mv $file.new $file
done

for file in $(find mods -name "*.lua");
do
	sed -i '1,26d' $file
	cat misc/header_lua.txt $file > $file.new && mv $file.new $file
done

