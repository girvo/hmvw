# Hacklang Blog/MVW Framework

## Introduction
This is the beginnings of a Model-View-Whatever Framework in Hack, the new 
PHP-derived statically (and strongly) typed web language. It's currently very 
rough, so feel free to take a look, but it's certainly only in it's earliest 
stage at the moment.

----

## Goals

### Everything As Strictly-Typed As Possible
Strict mode is the default, and only changed where absolutely necessary. No 
"mixed" or "nullable" unless it makes perfect sense to.

### Composition Over Inheritance
The aim is to allow for flexibility when building your web app; using Traits 
and Interfaces instead of regular class heirachies means you can have your app 
as complex as need be without the framework getting in the way. 

----

## Current Status
Very very rough. I have the base implementation using PDO of a "Model" and 
"Repository", and the base outline of how the router should work, and am in the
process of extracting the methods out into traits, and genericising them where 
I can.

----

## MIT License

Copyright (c) 2014 Joshua Girvin 

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 
