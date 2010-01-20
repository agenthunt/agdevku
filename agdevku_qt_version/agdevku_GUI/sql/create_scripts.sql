create database hello;

use hello;


create table department(
	dept_ID integer primary key,
	dept_Name varchar(40) not null
);

create index dept_index on
	department( dept_ID);
	
