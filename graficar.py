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


def graficarAyB(a, b, labelA, labelB, titulo, x, y):
	d5 = parsear(a)
	d6 = parsear(b)

	plt.plot(d5["threads"], d5["tiempo"], "r--", label=labelA)
	plt.plot(d6["threads"], d6["tiempo"], "g--", label=labelB)
	plt.axis([0, max(d5["threads"]+d6["threads"]), 0, max(d5["tiempo"]+d6["tiempo"])])
	plt.title(titulo)
	plt.xlabel(x)
	plt.ylabel(y)
	plt.show()


graficarAyB("maximum5-50-50","maximum6-50-50", "Maximum 5", "Maximum 6", "Maximum 5 y 6", "Threads", "Tiempo")
graficarAyB("countWords1-40-30","countWords2-40-30", "Count Words 1", "Count Words 2","Count Words 1 y 2", "Threads", "Tiempo")