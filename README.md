# Fuzzy Logic Library

- Developed by Pulkit Goyal

## Theory

Fuzzy logic is a type of control algorithm based on the use of membership functions and sets, a kind of controller that has an approach to computing based on "degrees of truth" rather than the usual boolean logic.
One of its main benefits is that it can incorporate a human being’s expert knowledge about how to control a system, without that a person need to have a mathematical description of the problem.
In our case fuzzy offers a more flexible relationship between the output w.r.t. to the error and difference between the error. On the other hand PID offers a linear relationship or a mathematical description of the relationship is required.

## Overview

A library with the main purpose to provide a number of functions to use in controlling the navigation of a robot in the domain of fuzzy logic.

## New Ideas and Main Features

- Automatically generated logic.
- Option to choose type of membership functions (Gaussian or Triangular)
- No limitation on the number of sets (with no need to define the logic for sets)
- Freedom to define discreet sets with varying standard deviation (or size in case of triangular) and centres.
- Appending sets at will at any point of time in the code (the code will adjust the set by performing a sorting operation).
- One line implementation while forming symmetric sets (which can be modified at will).

## Developments (from start to end)

The main task of the project was to create a library which allows to use functions in a typical fuzzy logic based control. As the project progressed, the feature of choosing the type of sets was incorporated. Also, a major development in the code was the recognition in the pattern of the logic which was the heart of the system which allowed the feature to use any number of sets which was a big accomplishment as at first the number of sets were limited by the logic matrix. At later stages of the project, the feature to use discreet/non-symmetric user defined sets was added which thus gave more control to the user.

## Usage

The main architecture of the library consists of a class with functions to create logic matrix, insert sets and use Gaussian or Triangular membership functions to calculate the correction. To use the library in an arduino based environment, the basic tasks are to create an object the append the sets by using either of the constructors or the `appendset()` function on the basis of usage. Then under the *loop()* heading, the error and the difference in error at each step is provided in the correction() function (which returns the correction) using the appropriate function type.
