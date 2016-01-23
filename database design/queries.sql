/*Names of all the actors in the movie 'Die Another Day': 
Halle Berry
Pierce Brosnan
John Cleese
Paul Darrow
Judi Dench
Thomas Ho
Michael Madsen
Rosamund Pike
Toby Stephens
Rick Yune
*/

SELECT first, last 
FROM Movie, MovieActor, Actor
WHERE Movie.id=MovieActor.mid AND title='Die Another Day' AND MovieActor.aid=Actor.id;



/*Names of movies that were released in 2003: 
All the Real Girls
Core, The
Deliver Us from Eva
Foreigner, The
Guest, The
Guy Thing, A
House of 1000 Corpses
Hunted, The
Inspector Gadget 2
Just Married
Kangaroo Jack
Levity
Life of David Gale, The
National Security
Old School
Recruit, The
Shape of Things, The
Special Forces
*/
SELECT title
FROM Movie
WHERE year=2003;

/*The count of all the actors who acted in multiple movies is 4824. */
SELECT COUNT(*)as number
FROM(
SELECT aid
FROM MovieActor
GROUP BY aid
HAVING COUNT(*)>=2) AS MA;