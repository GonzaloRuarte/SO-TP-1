import os
import shutil
import datetime


for i in range(0,50):
	src_dir="corpus"
	dst_dir="corpus" + str(i)
	shutil.copy(src_dir,dst_dir)