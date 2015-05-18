-- Filip Ježovica, xjezov01, 2015

DROP TABLE Student CASCADE CONSTRAINTS;
DROP TABLE Zapsal CASCADE CONSTRAINTS;
DROP TABLE Predmet CASCADE CONSTRAINTS;

CREATE TABLE Student (
	login NUMBER NOT NULL,
	meno VARCHAR2(255)
);

CREATE TABLE Zapsal (
	login NUMBER NOT NULL,
	zkr NUMBER NOT NULL,
  rok NUMBER NOT NULL
);

CREATE TABLE Predmet (
	zkr NUMBER NOT NULL,
	rok NUMBER NOT NULL,
  cviceni NUMBER NOT NULL
);
--PK
ALTER TABLE Student ADD CONSTRAINT PK_login PRIMARY KEY (login);
ALTER TABLE Predmet ADD CONSTRAINT PK_Pzkr PRIMARY KEY (zkr);

--FK
ALTER TABLE Zapsal ADD CONSTRAINT FK_Predmet FOREIGN KEY(zkr) REFERENCES Predmet;
ALTER TABLE Zapsal ADD CONSTRAINT FK_Student FOREIGN KEY(login) REFERENCES Student;

--INSERT
INSERT INTO Predmet VALUES('001', '2015','1');
INSERT INTO Predmet VALUES('002', '2015','1');
INSERT INTO Predmet VALUES('003', '2015','0');
INSERT INTO Predmet VALUES('004', '2015','0');

INSERT INTO Student VALUES('001', 'filip');
INSERT INTO Student VALUES('002', 'edo');
INSERT INTO Student VALUES('003', 'marek');
INSERT INTO Student VALUES('004', 'ja');

--STUDENT1 (len predmety s cviceniami)
INSERT INTO Zapsal VALUES('001', '001','2015');
INSERT INTO Zapsal VALUES('001', '002','2015');

--STUDENT2 (len predmety bez cviceni)
INSERT INTO Zapsal VALUES('002', '003','2015');
INSERT INTO Zapsal VALUES('002', '004','2015');

--STUDENT3 (aj cvicenia aj bez)
INSERT INTO Zapsal VALUES('003', '001','2015');
INSERT INTO Zapsal VALUES('003', '004','2015');

-- Vytvorte DB pohled ukazujici studenty, kteri maji zapsany mene nez ctyri predmety.
-- (ale maju aspoj jeden)
CREATE VIEW studenty AS
SELECT S.login, S.meno
FROM Student S, Zapsal Z
WHERE S.login = Z.login
GROUP BY S.login, S.meno
HAVING COUNT(Z.login) < 4;

-- "Kteri studenti (login, jmeno) maji letos zapsany pouze predmety, kde neni cviceni?"
SELECT DISTINCT S.meno FROM Student S, Zapsal Z WHERE S.login = Z.login AND Z.rok = 2015 AND NOT S.login IN
(SELECT DISTINCT login FROM Zapsal WHERE zkr IN
(SELECT zkr FROM Predmet WHERE cviceni = 1));


