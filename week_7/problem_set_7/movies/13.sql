SELECT DISTINCT name
FROM
(
SELECT movies.id AS movie_id
FROM movies
JOIN stars
ON movies.id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE people.name = 'Kevin Bacon'
 AND birth = 1958
) AS tmp
JOIN stars
ON tmp.movie_id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE name != 'Kevin Bacon';