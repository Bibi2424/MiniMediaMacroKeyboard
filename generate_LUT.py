import matplotlib.pyplot as plt
import numpy as np
import argparse




def valmap(value, istart, istop, ostart, ostop):
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart))

def listmap(l, istart, istop, ostart, ostop):
	return [valmap(i, istart, istop, ostart, ostop) for i in l]


def generate_LUT(function, elements, resolution, offset):

	if function == 'square':
		y = [0 if i < elements//2 else 2**resolution - 1 for i in range(elements)]

	elif function == 'ramp':
		y = [int(i/elements*2**resolution) for i in range(elements)]

	elif function == 'triangle':
		x = [int(2*i/elements*2**resolution) for i in range(elements//2)]
		if elements % 2 == 0:
			y = x + x[::-1]
		else:
			y = x + [2**resolution - 1] + x[::-1] 
			
	elif function == 'sin':
		t = np.arange(0, 2*np.pi, 2*np.pi / elements)
		x = np.sin(t)
		y = [int(i) for i in listmap(x, -1, 1, 0, 2**resolution)]
	
	elif function == 'cos':
		t = np.arange(0, 2*np.pi, 2*np.pi / elements)
		x = np.cos(t)
		y = [int(i) for i in listmap(x, -1, 1, 0, 2**resolution)]


	z = y[offset:] + y[:offset]

	return z



if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('--elements', '-n', default=256, type=int, help='Number of elements in the list')
	parser.add_argument('--resolution', '-r', default=8, type=int, help='Number of bits per value')
	parser.add_argument('--offset', '-o', default=0, type=int, help='horizontal offset')
	parser.add_argument('--function', '-f', default='sin', choices=['square', 'ramp', 'triangle', 'sin', 'cos'], help='types of pattern')
	parser.add_argument('--viz', action='store_true', help='previz the result')
	
	args = parser.parse_args()

	z = generate_LUT(args.function, args.elements, args.resolution, args.offset)

	print(len(z))
	print(z)

	if args.viz:
		plt.step(list(range(args.elements)), z, where='mid')
		plt.plot(list(range(args.elements)), z, 'o--', alpha = 0.3)

		plt.show()
