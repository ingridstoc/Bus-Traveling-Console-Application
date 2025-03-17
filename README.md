## Bus Traveling Console Application

## Description 

This is a console application for searching and booking bus trips. The application supports two types of users:

+ **Operator**: Can log in and manage bus routes (add/remove trips). The operator's account is predefined in the system and stored in a CSV file.
+ **User**: Can create an account, log in, search for bus trips, and make reservations. New users are stored in a CSV file.

## Features

+ **Operator functionalities**:
 -Login with existing credentials.
`-Add and remove trips.

+ **User functionalities**:
 -Create an account.
 -Login and authentication.
 -Search for available trips.
 -Make reservations.

+ **Exception Handling**:
 Operator:
 -Failed login (incorrect username or password).
 -Incorrect trip details (invalid date format, past dates, invalid city names).

User:
 -Failed login (incorrect email format, weak password, mismatched confirmation password).
 -Non-existent trip selection.
 -Incorrect reservation details.

+ **Security**:
User passwords are encrypted using the RSA algorithm.

## Requirements

 -C++ compiler 
 -Standard C++ Libraries
 
