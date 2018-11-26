##!/usr/bin/env python

from __future__ import print_function
import sys, optparse, subprocess, os, os.path, errno, zipfile, string, json, platform, shutil, tarfile, tempfile, multiprocessing, re, stat, copy
if sys.version_info >= (3,):
  pass
else:
  pass

env_dict={}

def env(key):
  if key in env_dict:
    return env_dict[key]
  else:
    return os.environ.get(key)

# def mock_env(key):
#   return "$$" + key + "$$"

def env_repl(matched_var):
  v = matched_var.group(1)
  # print("got:", v)
  return env(v)

def desc_expand(desc):
  return re.sub(r'@(.+?)@', env_repl, desc)

def main():
    for line in sys.stdin.readlines():
      sline = line.strip()
      if sline.startswith("#bazeldefine "):
        elems = [ e for e in sline.split(" ") if len(e) > 0 ]
        if env(elems[1]):
          print("#define " + elems[1] + ' ' + desc_expand(' '.join(elems[2:])))
        else:
          print("/* #undef " + elems[1] + ' */')
      else:
        print(desc_expand(line), end='')

if __name__ == '__main__':
    main()
