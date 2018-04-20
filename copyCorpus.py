import os
import shutil
import datetime
import random


for i in range(0,50):
	src_dir="corpus"
	dst_dir="Corpuses/corpus" + str(i)
	with open(src_dir, 'r') as source:
		data = [ ( random.random(),line) for line in source ]
	data.sort()
	with open(dst_dir, 'w') as target:
		for _, line in data:
			target.write( line )	
	
