/*
 * ============================================================================
 *  
 *        Authors: Christina SHI <hshi@cse.cuhk.edu.hk>
 *                 Kevin Yip <kevinyip@cse.cuhk.edu.hk>
 *
 * ============================================================================
 */

#Calculate ratio of true k-mers to false k-mers given sequencing length and base error rate

import argparse
import numpy as np
import re
import sys
import csv

def getArguments():
  parser = argparse.ArgumentParser(description="Calculate assemble statistics. #Either error_profile or E should be specified.")
  parser.add_argument('-K', '--kmer_len', required=True, help="k-mer length", type=int, default=0)
  parser.add_argument('-P', '--error_profile', help="error profile file, with each line specifying the average error rate in this base.", default="")
  parser.add_argument('-E', '--base_error_rate', help="base error rate on average.", type=float, default=-1)

  args = parser.parse_args()
  return args

#k states in total, state 0 means no error, state i means position of nearest base error to the sequence end. Each state represents a set of k-1 bases long sequences which serves as the prefix of a real k-mer.
def DP(K, error_profile):
  L=len(error_profile)
  trueKmer=0.0
  falseKmer=0.0
  states = [0.0]*(K+1)
  #initiate the start state
  states[0]=1
  for x in range(0, K):
    states[0] = states[0] * (1-error_profile[x])
  for x in range(0, K):
    states[x+1] = error_profile[x]
    for y in range(x+1, K, 1):
      states[x+1] = states[x+1] * (1-error_profile[y])
  
  trueKmer += states[0]
  falseKmer += (1-states[0])
  #processing to the end of sequence
  for x in range(K, L):
    newStates = [0.0]*(K+1)
    newStates[0] = states[0]*(1-error_profile[x])
    for y in range(1, K+1):
      tmp = states[y]*(1-error_profile[x])
      newStates[y-1] = newStates[y-1] + tmp
    newStates[K] = error_profile[x]
    
    trueKmer += newStates[0]
    falseKmer += (1-newStates[0])
    states = newStates
  return [trueKmer, falseKmer]

args = getArguments()
#check the arguments
if args.error_profile =="" and args.base_error_rate == -1:
  print "Please specify either the error profile(P) file or the average base error rate(E)"
  sys.exit(0)

if args.error_profile != "":
  with open(args.error_profile, 'r') as fin:
    reader = csv.reader(fin, delimiter=',')
    error_profile = list(reader)
    error_profile = [float(x[0]) for x in error_profile]
  
    [trueKmer, falseKmer] = DP(args.kmer_len, error_profile)

    print "trueKmer/falseKmer = ", trueKmer, "/", falseKmer, " = ", trueKmer/falseKmer
else:
  trueKmer = pow(1-args.E, args.K)
  falseKmer = 1-trueKmer
  print "trueKmer/falseKmer = ", trueKmer, "/", falseKmer, " = ", trueKmer/falseKmer
  

