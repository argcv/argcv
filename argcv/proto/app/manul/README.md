# Manul

## Brief

**It is still on a very early stage.**

Manul is a docker based auto grading system.

Please refer to [this repo](https://github.com/argcv/manul) for the implementation of the project.


## Overview

`WIP`

## Basic Functions

+ For Projects <project.proto>
    + List projects
    + Create project
    + Update project
        + Update project info
        + Update user
    + Delete project

+ For Users <user.proto>
    + List users
    + Add user
    + Update user
    + Delete user
    + Generate token

+ For Authorization <secret.proto>
    + Update password -- given global password/old password/old token, set token for specific project
    + Forget password -- send a email to self with your token, which could be used to update *global* password

+ For Jobs <job.proto>
    + List jobs
    + Submit job
    + Check status
    + For Access Tokens
        + Verify token (given user + token + project, return true or false)

