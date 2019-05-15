#!/bin/bash
#SBATCH --job-name=Group10
#SBATCH --output=cell_%j.out
#SBATCH --error=cell_%j.err
#SBATCH --ntasks=40
#SBATCH --cpus-per-task=4
srun ./simpar-ompi 2 10 100000000 5
srun ./simpar-ompi 2 20 100000000 5
srun ./simpar-ompi 2 30 100000000 5
srun ./simpar-ompi 2 40 100000000 5
srun ./simpar-ompi 2 50 100000000 5
srun ./simpar-ompi 2 60 100000000 5
srun ./simpar-ompi 2 70 100000000 5
srun ./simpar-ompi 2 90 100000000 5
srun ./simpar-ompi 2 100 100000000 5
srun ./simpar-ompi 2 250 100000000 5
srun ./simpar-ompi 2 500 100000000 5
srun ./simpar-ompi 2 1000 100000000 5

