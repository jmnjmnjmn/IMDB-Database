CREATE TABLE Movie(
	id int,
	title varchar(100) NOT NULL,/*movie must have a title*/
	year int,
	rating varchar(10),
	company varchar(50),
	PRIMARY KEY(id) /*uniqe Movie id*/
	/*CHECK(year>1900)*/ /* movie should be later than 1900 */
	)ENGINE=InnoDB;    
CREATE TABLE Actor(
	id int,
	last varchar(20),
	first varchar(20),
	sex varchar(6),
	dob DATE,
	dod DATE,
	PRIMARY KEY(id) /*uniqe Actor id*/
	/*CHECK(last<>NULL or first<>NULL)*/ /* actor must have a name */
	/*CHECK(dob<dod)*/  /* actor's dob must earlier than dod */
	)ENGINE=InnoDB;
CREATE TABLE Director(
	id int,
	last varchar(20),
	first varchar(20),
	dob DATE,
	dod DATE,
	PRIMARY KEY(id) /*uniqe Director id*/
	/*CHECK(last<>NULL or first<>NULL)*//* Director must have a name */
	/*CHECK(dob<dod)*/ /* actor's dob must earlier than dod */
	)ENGINE=InnoDB;
CREATE TABLE MovieGenre(
	mid int,
	genre varchar(20),
	FOREIGN KEY (mid) references Movie(id))ENGINE=INNODB;  /* mid must in Movie table's id */
CREATE TABLE MovieDirector(
	mid int,
	did int,
	FOREIGN KEY (mid) references Movie(id), /* mid must in Movie table's id */
	FOREIGN KEY (did) references Director(id))ENGINE=INNODB; /* did must in director table's id */
CREATE TABLE MovieActor(
	mid int,
	aid int,
	role varchar(50)NOT NULL,/*each actor must have a role*/
	FOREIGN KEY (mid) references Movie(id),/* mid must in Movie table's id */
	FOREIGN KEY (aid) references Actor(id))ENGINE=INNODB;/* did must in acotr table's id */
CREATE TABLE Review(
	name varchar(20),
	time TIMESTAMP,
	mid int,
	rating int NOT NULL,/*review must have a rating*/
	comment varchar(500),
	FOREIGN KEY (mid) references Movie(id))ENGINE=INNODB;/* mid must in Movie table's id */
CREATE TABLE MaxPersonID(id int)ENGINE=InnoDB;
CREATE TABLE MaxMovieID(id int)ENGINE=InnoDB;


