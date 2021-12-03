SELECT *
FROM
(
SELECT title
FROM movies
JOIN stars
ON movies.id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE name = 'Johnny Depp'
) AS d
JOIN
(
SELECT title
FROM movies
JOIN stars
ON movies.id = stars.movie_id
JOIN people
ON stars.person_id = people.id
WHERE name = 'Helena Bonham Carter'
) AS h
ON d.title = h.title;