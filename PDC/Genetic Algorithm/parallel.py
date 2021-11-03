import random
import threading
import math

POPULATION_SIZE = 100
GENES = '''ãªabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ 1234567890,.;:—_©!"#%^&*()=?@§$-~=+\|/{[]}`¨“´‘’”'''
f = open("./ShortStories/50.txt", "r")

TARGET = f.read()

class Individual(object):
	def __init__(self, chromosome):
		self.chromosome = chromosome
		self.fitness = self.cal_fitness()

	@classmethod
	def mutated_genes(self):
		global GENES
		gene = random.choice(GENES)
		return gene

	@classmethod
	def create_gnome(self):
		global TARGET
		gnome_len = len(TARGET)
		return [self.mutated_genes() for _ in range(gnome_len)]

	def mate(self, par2):

		child_chromosome = []
		for gp1, gp2 in zip(self.chromosome, par2.chromosome):	

			prob = random.random()

			if prob < 0.45:
				child_chromosome.append(gp1)

			elif prob < 0.90:
				child_chromosome.append(gp2)

			else:
				child_chromosome.append(self.mutated_genes())

		return Individual(child_chromosome)

	def cal_fitness(self):
		global TARGET
		fitness = 0
		for gs, gt in zip(self.chromosome, TARGET):
			if gs != gt: fitness+= 1
		return fitness

exitFlag = 0

class myThread(threading.Thread):
  def __init__(self, threadID, generation, s):
    threading.Thread.__init__(self)
    self.threadID = threadID
    self.generation = generation
    self.s = s
  def run(self):
    for _ in range(self.s):
      parent1 = random.choice(population[:20])
      parent2 = random.choice(population[:20])
      child = parent1.mate(parent2)
      new_generation.append(child)
    

# Driver code
def main():
	global POPULATION_SIZE


	#current generation
	generation = 1

	found = False
	global population 
	population = []
	newFitness = math.inf
	# create initial population
	for _ in range(POPULATION_SIZE):
		gnome = Individual.create_gnome()
		population.append(Individual(gnome))

	while not found:

		# sort the population in increasing order of fitness score
		population = sorted(population, key = lambda x:x.fitness)

		# if the individual having lowest fitness score ie.
		# 0 then we know that we have reached to the target
		# and break the loop
		if population[0].fitness <= 0:
			found = True
			break

		# Otherwise generate new offsprings for new generation
		global new_generation
		new_generation = []

		# Perform Elitism, that mean 10% of fittest population
		# goes to the next generation
		s = int((10*POPULATION_SIZE)/100)
		new_generation.extend(population[:s])

		# From 50% of fittest population, Individuals
		# will mate to produce offspring
		s = int((90*POPULATION_SIZE)/100)

		thread1 = myThread(1, generation, s)
		thread2 = myThread(2, generation, s)
		thread3 = myThread(3, generation, s)
		thread4 = myThread(4, generation, s)
		thread5 = myThread(5, generation, s)

    # Start new Threads
		thread1.start()
		thread2.start()
		thread3.start()
		thread4.start()
		thread5.start()

		thread1.join()
		thread2.join()
		thread3.join()
		thread4.join()
		thread5.join()

		population = new_generation
		if(population[0].fitness is not newFitness):
		  print("Generation: ", generation, "\tFitness: ",population[0].fitness)
		  newFitness = population[0].fitness

		# print("Generation: {}\tString: {}\tFitness: {}".\
		# 	format(generation,
		# 	"".join(population[0].chromosome),
		# 	population[0].fitness))

		generation += 1

	print("Generation: ", generation, "\tFitness: ",population[0].fitness)
	# print("Generation: {}\tString: {}\tFitness: {}".\
	# 	format(generation,
	# 	"".join(population[0].chromosome),
	# 	population[0].fitness))

if __name__ == '__main__':
	main()
