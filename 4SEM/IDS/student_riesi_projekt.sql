DROP TABLE Student CASCADE CONSTRAINTS;
DROP TABLE Resi CASCADE CONSTRAINTS;
DROP TABLE Projekt CASCADE CONSTRAINTS;

CREATE TABLE Student (
	login NUMBER NOT NULL,
	meno VARCHAR2(255),
  rocnik NUMBER NOT NULL
);

CREATE TABLE Resi (
	login NUMBER NOT NULL,
	cislo NUMBER NOT NULL
);

CREATE TABLE Projekt (
	cislo NUMBER NOT NULL,
  nazev VARCHAR2(255)
);
--PK
ALTER TABLE Student ADD CONSTRAINT PK_login PRIMARY KEY (login);
ALTER TABLE Projekt ADD CONSTRAINT PK_Projzkr PRIMARY KEY (cislo);

--FK
ALTER TABLE Resi ADD CONSTRAINT FK_Projekt FOREIGN KEY(cislo) REFERENCES Projekt;
ALTER TABLE Resi ADD CONSTRAINT FK_Student FOREIGN KEY(login) REFERENCES Student;

--INSERT
INSERT INTO Projekt VALUES('001', 'PROJEKT1');
INSERT INTO Projekt VALUES('002', 'PROJEKT2');
INSERT INTO Projekt VALUES('003', 'PROJEKT3');
INSERT INTO Projekt VALUES('004', 'PROJEKT4');

INSERT INTO Student VALUES('001', 'filip', '3');
INSERT INTO Student VALUES('002', 'edo', '3');
INSERT INTO Student VALUES('003', 'marek', '3');
INSERT INTO Student VALUES('004', 'ja', '1');

--STUDENT1             ( stud,  Proj  )
INSERT INTO Resi VALUES('001', '001');
INSERT INTO Resi VALUES('001', '002');

--STUDENT2
INSERT INTO Resi VALUES('002', '001');
INSERT INTO Resi VALUES('002', '003');

--STUDENT3
INSERT INTO Resi VALUES('003', '001');
INSERT INTO Resi VALUES('003', '004');

--STUDENT4
INSERT INTO Resi VALUES('004', '001');
INSERT INTO Resi VALUES('004', '004');


-- projekty, ktore robili vsetci 3tieho rocniku
SELECT nazev FROM Projekt NATURAL JOIN Resi NATURAL JOIN Student
WHERE rocnik = 3 group by nazev
HAVING count(login) = (SELECT COUNT(login) FROM Student WHERE rocnik = 3 );
-- ten koniec spocita vsetkych studentov 3 rocnika
-- count(login) suvisi stym hore - napocita kolko studentov robilo dany projekt
-- no a ked sa rovnaju tak to su presne tie projekty co robili vsetci




