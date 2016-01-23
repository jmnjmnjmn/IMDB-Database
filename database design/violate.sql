/*Violate 3 Primary key*/

INSERT INTO Movie VALUES(2, 'ME', 1996, 'R','LA');/* --Duplicate entry for key '2' for key 1*/
Update Actor set id=1 where id=10; /*--Duplicate entry for key '1' for key 1*/
Update Director set id=16 where id=63; /*--Duplicate entry for key '16' for key 1*/


/*Violate 6 Foreign key*/

INSERT INTO MovieGenre VALUES(10, 'Drama'); /*--Cannot add or update a child row: a foreign key constraint fails*/

INSERT INTO MovieDirector VALUES(10, 16); /*--Cannot add or update a child row: a foreign key constraint fails*/
INSERT INTO MovieDirector VALUES(12, 17); /*--Cannot add or update a child row: a foreign key constraint fails*/

INSERT INTO MovieActor VALUES(10, 10, 'Katy Perry'); /*--Cannot add or update a child row: a foreign key constraint fails*/
INSERT INTO MovieActor VALUES(12, 2, 'Katy Perry'); /*--Cannot add or update a child row: a foreign key constraint fails*/

INSERT INTO Review VALUES('Lee', 2013-01-19, 10, 100, 'Good'); /*--Cannot add or update a child row: a foreign key constraint fails*/


/*Violate 3 Constraints*/

UPDATE Movie SET year=1800
WHERE id=2; /*--Violate the constraint 'year>1900'*/

UPDATE Actor SET last=NULL, first=NULL
WHERE id=1; /*--Violate the constraint that first name and last name should not be NULL simultaneously*/

UPDATE Director SET last=NULL, first=NULL
WHERE id=16; /*--Violate the constraint that first name and last name should not be NULL simultaneously*/
