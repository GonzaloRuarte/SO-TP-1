import matplotlib.pyplot as plt

def parsear(fileName):
	with open(fileName) as f:
	    content = f.readlines()
	fileList = [x.strip() for x in content]

	threads = []
	tiempo = []
	for results in fileList:
		threads.append(int((results.split(" "))[0]))
		tiempo.append(int((results.split(" "))[1]))

	return {"threads":threads, "tiempo":tiempo}


def graficarAyB(b, labelB, titulo, x, y):
	d6 = parsear(b)

	plt.plot(d6["threads"], d6["tiempo"], "ro", label=labelB)
	plt.axis([0, max(d6["threads"]), 0, max(d6["tiempo"])])
	plt.title(titulo)
	plt.xlabel(x)
	plt.ylabel(y)
	plt.show()


graficarAyB("countWords2-40-30", "Count Words 2","Count Words 2", "Threads", "Tiempo")
