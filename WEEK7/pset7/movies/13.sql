SELECT people.name  FROM people
INNER JOIN stars ON people.id = stars.person_id
WHERE movie_id IN (
SELECT movie_id FROM stars
INNER JOIN people ON stars.person_id = people.id
WHERE people.name = "Kevin Bacon" AND people.birth = 1958)
AND NOT people.name = "Kevin Bacon";