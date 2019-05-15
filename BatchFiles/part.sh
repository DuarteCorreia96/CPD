#!/bin/bash
#SBATCH --job-name=Group10
#SBATCH --output=part_%j.out
#SBATCH --error=part_%j.err
#SBATCH --ntasks=40
#SBATCH --cpus-per-task=4
srun ./simpar-ompi 2 20 100 5
srun ./simpar-ompi 2 20 1000 5
srun ./simpar-ompi 2 20 10000 5
srun ./simpar-ompi 2 20 100000 5
srun ./simpar-ompi 2 20 1000000 5
srun ./simpar-ompi 2 20 10000000 5
srun ./simpar-ompi 2 20 100000000 5
srun ./simpar-ompi 2 20 1000000000 5

