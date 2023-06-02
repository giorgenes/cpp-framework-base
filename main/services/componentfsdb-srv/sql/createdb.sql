drop table revisions;
drop table components;

create table components (
	id SERIAL,
	name varchar(128) NOT NULL,
	UNIQUE(name),
	PRIMARY KEY(id)
);

create table revisions (
	name varchar(64),
	next integer,
	component integer REFERENCES components (id),
	PRIMARY KEY(name, component)
);

