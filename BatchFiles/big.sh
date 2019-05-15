#!/bin/bash
#SBATCH --job-name=Group10
#SBATCH --output=cell_%j.out
#SBATCH --error=cell_%j.err
#SBATCH --ntasks=80
#SBATCH --cpus-per-task=4
srun ./simpar-ompi 5 200 300000000 2
srun ./simpar-ompi 8 1500 200000000 5


