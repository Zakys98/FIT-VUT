DROP TABLE uzivatel CASCADE CONSTRAINTS;
DROP TABLE objednavka CASCADE CONSTRAINTS;
DROP TABLE stul CASCADE CONSTRAINTS;
DROP TABLE rezervace CASCADE CONSTRAINTS;
DROP TABLE zaznam_platby CASCADE CONSTRAINTS;
DROP TABLE polozka_menu CASCADE CONSTRAINTS;
DROP TABLE objednavka_polozka CASCADE CONSTRAINTS;
DROP TABLE alergen CASCADE CONSTRAINTS;
DROP TABLE surovina CASCADE CONSTRAINTS;
DROP TABLE alergen_surovina CASCADE CONSTRAINTS;
DROP TABLE rezervace_stul CASCADE CONSTRAINTS;
DROP TABLE polozka_menu_surovina CASCADE CONSTRAINTS;
DROP TABLE rezervace_objednavka CASCADE CONSTRAINTS;
DROP TABLE objednavka_zaznam CASCADE CONSTRAINTS;

CREATE TABLE uzivatel (
    id                  INT         PRIMARY KEY,
    jmeno               varchar(30),
    prijmeni            varchar(30),
    role                varchar(15)
);
CREATE TABLE objednavka (
    id                  INT        GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    cas_vytvoreni       DATE       NOT NULL,
    cena                NUMBER,
    id_stolu            INT        NOT NULL,
    id_uzivatele        INT        NOT NULL
);
CREATE TABLE stul (
    id                  INT         GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    mistnost            varchar(15),
    max_osob            number(2)      NOT NULL
);
CREATE TABLE rezervace_stul(
    id_rezervace        INT         NOT NULL,
    id_stolu            INT         NOT NULL,
    PRIMARY KEY (id_rezervace, id_stolu)
);
CREATE TABLE rezervace(
    id                  INT         GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    datum               DATE        NOT NULL,
    jmeno               varchar(30) NOT NULL,
    email               varchar(30),
    tel_cislo           varchar(13),
    id_uzivatele        INT         NOT NULL,
    pocet_osob          NUMBER(2)   NOT NULL
);
CREATE TABLE rezervace_objednavka(
    id_rezervace        INT         NOT NULL,
    id_objednavka       INT         NOT NULL,
    PRIMARY KEY (id_objednavka, id_rezervace)
);
CREATE TABLE zaznam_platby(
    id                  INT         GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    cislo_pobocky       number(3)   NOT NULL,
    cislo_pokladny      number(4)   NOT NULL,
    datum               DATE        NOT NULL,
    rezim_platby        varchar(20) DEFAULT 'bezny',
    FIK                 char(39),
    PKP                 varchar(334)DEFAULT NULL,
    BKP                 char(44),
    DIC                 char(12),
    id_uzivatele        INT        NOT NULL,
    celkova_cena        number     NOT NULL
);
CREATE TABLE objednavka_zaznam(
    id_objednavka       INT        NOT NULL,
    id_zaznam           INT        NOT NULL,
    PRIMARY KEY (id_zaznam, id_objednavka)
);
CREATE TABLE objednavka_polozka(
    id                  INT         GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    id_objednavky       INT         NOT NULL,
    id_polozky          INT         NOT NULL,
    detska_porce        char(1)     DEFAULT 'N'
);
CREATE TABLE alergen(
    id                  INT         GENERATED BY DEFAULT AS IDENTITY PRIMARY KEY,
    nazev               varchar(40) UNIQUE NOT NULL
);
CREATE TABLE surovina(
     id                 INT         GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
     nazev              varchar(20) UNIQUE NOT NULL,
     mnozstvi_sklad     varchar(10)
);
CREATE TABLE alergen_surovina(
    id_alergenu         INT         NOT NULL,
    id_suroviny         INT         NOT NULL,
    PRIMARY KEY (id_alergenu, id_suroviny)
);
CREATE TABLE polozka_menu_surovina(
    id_polozky          INT         NOT NULL,
    id_suroviny         INT         NOT NULL,
    mnozstvi            varchar(10),
    PRIMARY KEY (id_polozky, id_suroviny)
);
CREATE TABLE polozka_menu(
    id                  INT         GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
    nazev               varchar(30) UNIQUE NOT NULL,
    cena                number(3)   NOT NULL,
    hmotnost_jidla      number(3),
    objem_napoje        number(3,2),
    alkoholicky         char(1)     DEFAULT 'N'
);

ALTER TABLE zaznam_platby ADD CONSTRAINT CHECK_celkova_cena CHECK (celkova_cena > 0);
ALTER TABLE polozka_menu ADD CONSTRAINT CHECk_cena CHECK (cena > 0);
ALTER TABLE uzivatel ADD CONSTRAINT CHECK_role check(role in ('zamestnanec', 'vedouci', 'majitel'));
ALTER TABLE polozka_menu ADD CONSTRAINT CHECK_alkohol check (alkoholicky in ('A', 'N'));
ALTER TABLE objednavka_polozka ADD CONSTRAINT CHECK_detska check (detska_porce in ('A', 'N'));
ALTER TABLE zaznam_platby ADD CONSTRAINT CHECK_rezim_platby check (rezim_platby in ('bezny', 'zjednoduseny'));

ALTER TABLE objednavka ADD CONSTRAINT FK_objednavka_stul FOREIGN KEY (id_stolu) REFERENCES stul (id);
ALTER TABLE objednavka ADD CONSTRAINT FK_objednavka_uzivatel FOREIGN KEY (id_uzivatele) REFERENCES uzivatel (id);

ALTER TABLE rezervace_objednavka ADD CONSTRAINT FK_rezervace_objednavka_rezervace FOREIGN KEY (id_rezervace) REFERENCES rezervace (id) ON DELETE CASCADE;
ALTER TABLE rezervace_objednavka ADD CONSTRAINT FK_rezervace_objednavka_objednavka FOREIGN KEY (id_objednavka) REFERENCES objednavka (id) ON DELETE CASCADE;

ALTER TABLE rezervace_stul ADD CONSTRAINT FK_rezervace_stul_rezervace FOREIGN KEY (id_rezervace) REFERENCES rezervace (id) ON DELETE CASCADE;
ALTER TABLE rezervace_stul ADD CONSTRAINT FK_rezervace_stul_stul FOREIGN KEY (id_stolu) REFERENCES stul (id) ON DELETE CASCADE;

ALTER TABLE rezervace ADD CONSTRAINT FK_rezervace_uzivatel FOREIGN KEY (id_uzivatele) REFERENCES uzivatel (id);

ALTER TABLE objednavka_zaznam ADD CONSTRAINT FK_objednavka_zaznam_objednavka FOREIGN KEY (id_objednavka) REFERENCES objednavka (id) ON DELETE CASCADE;
ALTER TABLE objednavka_zaznam ADD CONSTRAINT FK_objednavka_zaznam_zaznam FOREIGN KEY (id_zaznam) REFERENCES zaznam_platby (id) ON DELETE CASCADE;

ALTER TABLE zaznam_platby ADD CONSTRAINT FK_zaznam_platby_uzivatel FOREIGN KEY (id_uzivatele) REFERENCES uzivatel (id);

ALTER TABLE alergen_surovina ADD CONSTRAINT FK_alergen_surovina_alergen FOREIGN KEY (id_alergenu) REFERENCES alergen (id) ON DELETE CASCADE;
ALTER TABLE alergen_surovina ADD CONSTRAINT FK_alergen_surovina_surovina FOREIGN KEY (id_suroviny) REFERENCES surovina (id) ON DELETE CASCADE;

ALTER TABLE polozka_menu_surovina ADD CONSTRAINT FK_polozka_menu_surovina_polozka FOREIGN KEY (id_polozky) REFERENCES polozka_menu (id) ON DELETE CASCADE;
ALTER TABLE polozka_menu_surovina ADD CONSTRAINT FK_polozka_menu_surovina_surovina FOREIGN KEY (id_suroviny) REFERENCES surovina (id) ON DELETE CASCADE;

ALTER TABLE objednavka_polozka ADD CONSTRAINT FK_obsahuje_polozka FOREIGN KEY (id_polozky) REFERENCES polozka_menu (id) ON DELETE CASCADE;
ALTER TABLE objednavka_polozka ADD CONSTRAINT FK_obsahuje_objednavka FOREIGN KEY (id_objednavky) REFERENCES objednavka (id) ON DELETE CASCADE;

-- trigger sluzi na generovanie id cisla, ak ho uzivatel nezada
CREATE OR REPLACE TRIGGER generate_uzivatel_id
	BEFORE INSERT ON uzivatel
    FOR EACH ROW
    WHEN (NEW.id IS NULL)
DECLARE
	max_id	uzivatel.id%TYPE;
BEGIN

    SELECT MAX(id) INTO max_id
	FROM uzivatel;

    IF max_id IS NULL THEN
        max_id := 1;
    ELSE
        max_id := max_id + 1;
    END IF;

    :NEW.id := max_id;
END;
/

-- kontrola emailu, telefonu a jmena v tabulce rezervace
CREATE OR REPLACE TRIGGER trig_check_rezervation
    BEFORE INSERT OR UPDATE OF JMENO, EMAIL, TEL_CISLO ON REZERVACE
    FOR EACH ROW
BEGIN
    IF NOT REGEXP_LIKE(:NEW.TEL_CISLO, '^\+[0-9]{12}$') THEN
        RAISE_APPLICATION_ERROR(-20001, 'Špatný formát tel čísla');
    END IF;
    IF LENGTH(:NEW.JMENO)>30 THEN
        RAISE_APPLICATION_ERROR(-20002, 'Špatná délka jména');
    END IF;
    IF NOT REGEXP_LIKE(:NEW.JMENO, '^[A-ZÁŽŠČŘ]{1}[a-zážěščřýíéůúóďť]+ [A-ZÁŽŠČŘ]{1}[a-zážěščřýíéůúóďť]+$') THEN
        RAISE_APPLICATION_ERROR(-20003, 'Špatný formát jména');
    END IF;
    IF NOT REGEXP_LIKE(:NEW.EMAIL, '^[a-zA-Z0-9-_.]+@[a-z0-9-_]+\.[a-z]{2,4}$') THEN
        RAISE_APPLICATION_ERROR(-20004, 'Špatný formát emailu');
    END IF;
END;
/

-- pocedura vypocita celkovu cenu poloziek objednavky aj s uplatnenim zlavy na detsku porciu
-- vstupny parameter: cislo objednavky
-- ak objednavka neexistuje, nevykona sa nic
CREATE OR REPLACE PROCEDURE set_objednavka_cena (objednavka_id IN NUMBER)
IS
    celkova_cena    NUMBER := 0;
	cena	        polozka_menu.cena%TYPE;
	porce	        objednavka_polozka.detska_porce%TYPE;

    CURSOR polozka_objednavky IS
	SELECT pm.cena, op.detska_porce
	FROM objednavka_polozka op, polozka_menu pm
	WHERE op.id_objednavky = objednavka_id and op.id_polozky = pm.id ;

BEGIN
	OPEN polozka_objednavky;
	LOOP
		FETCH polozka_objednavky INTO cena, porce;
		EXIT WHEN polozka_objednavky%NOTFOUND;

		IF porce = 'A' THEN
			celkova_cena := celkova_cena + (0.7*cena);
		ELSE
			celkova_cena := celkova_cena + cena;
		END IF;
	END LOOP;
	CLOSE polozka_objednavky;
	UPDATE objednavka SET cena = ROUND(celkova_cena)
	WHERE id = objednavka_id;
END;
/

-- trigger sa spusti po pridani polozky do objednavky
-- a aktualizuje cenu objednavky
CREATE OR REPLACE TRIGGER vypocet_ceny_objednavky
FOR INSERT ON objednavka_polozka
COMPOUND TRIGGER
    obj_id   objednavka.id%TYPE;

    AFTER EACH ROW IS
    BEGIN
        obj_id := :NEW.id_objednavky;
    END AFTER EACH ROW;

    AFTER STATEMENT IS
    BEGIN
        set_objednavka_cena(obj_id);
    END AFTER STATEMENT;
END vypocet_ceny_objednavky;
/

--    kdyz se smaze uzivatel, tak se zmeni id v zaznamu_platby na id majitele
CREATE OR REPLACE PROCEDURE proc_delete_user(id_arg in integer) AS
    CURSOR zaznam is SELECT id_uzivatele FROM ZAZNAM_PLATBY;
    CURSOR obj is SELECT id_uzivatele FROM OBJEDNAVKA;
    CURSOR rez is SELECT id_uzivatele FROM REZERVACE;
    zaz ZAZNAM_PLATBY.id_uzivatele%TYPE;
    id_zaz      UZIVATEL.id%TYPE;
    id_majitel  UZIVATEL.id%TYPE;
BEGIN
    id_zaz := -1;
    id_majitel := -1;
    SELECT id INTO id_zaz FROM UZIVATEL WHERE UZIVATEL.ID = id_arg;
    SELECT id INTO id_majitel FROM UZIVATEL WHERE UZIVATEL.ROLE = 'majitel';
    IF id_majitel = id_zaz THEN
        RAISE_APPLICATION_ERROR(-20000, 'Nemuzete smazat majitele');
    END IF;
    OPEN zaznam;
    LOOP
      FETCH zaznam INTO zaz;
      EXIT WHEN zaznam%NOTFOUND;
      IF zaz = id_zaz THEN
            UPDATE ZAZNAM_PLATBY SET ID_UZIVATELE = id_majitel WHERE ID_UZIVATELE = id_zaz;
      END IF;
    END LOOP;
    OPEN obj;
    LOOP
      FETCH obj INTO zaz;
      EXIT WHEN obj%NOTFOUND;
      IF zaz = id_zaz THEN
            UPDATE OBJEDNAVKA SET ID_UZIVATELE = id_majitel WHERE ID_UZIVATELE = id_zaz;
      END IF;
    END LOOP;
    OPEN rez;
    LOOP
      FETCH rez INTO zaz;
      EXIT WHEN rez%NOTFOUND;
      IF zaz = id_zaz THEN
            UPDATE REZERVACE SET ID_UZIVATELE = id_majitel WHERE ID_UZIVATELE = id_zaz;
      END IF;
    END LOOP;
    DELETE FROM UZIVATEL WHERE UZIVATEL.id = id_zaz;
EXCEPTION
    WHEN NO_DATA_FOUND THEN
        BEGIN
            IF id_zaz = -1 THEN RAISE_APPLICATION_ERROR(-20000, 'Uzivatel nenalezen'); END IF;
            IF id_majitel = -1 THEN RAISE_APPLICATION_ERROR(-20000, 'Majitel nenalezen'); END IF;
        END;
END;
/

INSERT INTO alergen(nazev) VALUES ('Lepek');
INSERT INTO alergen(nazev) VALUES ('Korýši');
INSERT INTO alergen(nazev) VALUES ('Vejce');
INSERT INTO alergen(nazev) VALUES ('Ryby');
INSERT INTO alergen(nazev) VALUES ('Arašídy');
INSERT INTO alergen(nazev) VALUES ('Sója');
INSERT INTO alergen(nazev) VALUES ('Mléko');
INSERT INTO alergen(nazev) VALUES ('Skořápkové plody');
INSERT INTO alergen(nazev) VALUES ('Celer');
INSERT INTO alergen(nazev) VALUES ('Hořčice');
INSERT INTO alergen(nazev) VALUES ('Sezam');
INSERT INTO alergen(nazev) VALUES ('Oxid siřičitý a siřičitany');
INSERT INTO alergen(nazev) VALUES ('Vlčí bob');
INSERT INTO alergen(nazev) VALUES ('Měkkýši');

INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Mléko', '5 litrů');
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Arašídy', '5 kg');
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Celer', '10 kg');
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Tofu', '5 kg');
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Olivový olej', '10 litrů');

INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (5, 2);
INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (7, 1);
INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (9, 3);
INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (6, 4);

INSERT INTO polozka_menu(nazev, cena, hmotnost_jidla) VALUES ('Salát', 200, 200);
INSERT INTO polozka_menu(nazev, cena, objem_napoje) VALUES ('Mléko', 30, 0.5);
INSERT INTO polozka_menu(nazev, cena, hmotnost_jidla) VALUES ('Tofu steak', 90, 200);
INSERT INTO polozka_menu(nazev, cena, objem_napoje) VALUES ('Cappy jahodový', 55, 0.33);

INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (2, 1, '0.5 litrů');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (1, 2, '0.05 kg');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (1, 3, '0.15 kg');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (3, 4, '0.25 kg');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (3, 5, '5 ml');

INSERT INTO uzivatel(jmeno, prijmeni, role) VALUES ('Pavel', 'Novák', 'zamestnanec');
INSERT INTO uzivatel(jmeno, prijmeni, role) VALUES ('Dominik', 'Vašut', 'vedouci');
INSERT INTO uzivatel(jmeno, prijmeni, role) VALUES ('Jana', 'Žáková', 'majitel');
INSERT INTO uzivatel(jmeno, prijmeni, role) VALUES ('Peter', 'Hollý', 'zamestnanec');
INSERT INTO uzivatel(jmeno, prijmeni, role) VALUES ('Andrej', 'Černý', 'zamestnanec');

INSERT INTO stul(mistnost, max_osob) VALUES ('salonek',5);
INSERT INTO stul(mistnost, max_osob) VALUES ('salonek',8);
INSERT INTO stul(mistnost, max_osob) VALUES ('sal',10);
INSERT INTO stul(mistnost, max_osob) VALUES ('sal',8);
INSERT INTO stul(mistnost, max_osob) VALUES ('terasa',6);
INSERT INTO stul(mistnost, max_osob) VALUES ('terasa',5);

INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2010-01-03 18:00', 'yyyy-mm-dd hh24:mi'), 'Petr Švec', 'petr.svec@gmail.com', '+235603965234', 1, 5);
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2010-05-01 17:00', 'yyyy-mm-dd hh24:mi'), 'Kamila Hánová', 'hanova@seznam.cz', '+421732968715', 2, 4);
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2020-04-01 19:00', 'YYYY-MM-DD HH24:MI'),'Ján Podhorský', 'j.podhor@gmail.com', '+420111111111', 4, 3);
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2020-04-05 18:30', 'YYYY-MM-DD HH24:MI'),'Erik Jánoš', 'bingo223@seznam.cz', '+420633266777', 4, 4);

INSERT INTO rezervace_stul(id_rezervace, id_stolu) VALUES (1,1);
INSERT INTO rezervace_stul(id_rezervace, id_stolu) VALUES (2,3);
INSERT INTO rezervace_stul(id_rezervace, id_stolu) VALUES (3,6);
INSERT INTO rezervace_stul(id_rezervace, id_stolu) VALUES (4,6);

INSERT INTO objednavka(cas_vytvoreni, cena, id_stolu, id_uzivatele) VALUES (TO_DATE('2010-01-03 18:15:50', 'yyyy-mm-dd hh24:mi:ss'), 400, 1, 1);
INSERT INTO objednavka(cas_vytvoreni, cena, id_stolu, id_uzivatele) VALUES (TO_DATE('2010-05-01 17:10:24', 'yyyy-mm-dd hh24:mi:ss'), 260, 3, 1);
INSERT INTO objednavka(cas_vytvoreni, cena, id_stolu, id_uzivatele) VALUES (TO_DATE('2020-03-21 09:10:22', 'YYYY-MM-DD HH24:MI:SS'), 90, 5, 5);
INSERT INTO objednavka(cas_vytvoreni, cena, id_stolu, id_uzivatele) VALUES (TO_DATE('2020-04-05 18:35:54', 'YYYY-MM-DD HH24:MI:SS'), 145, 6, 4);

INSERT INTO rezervace_objednavka(id_rezervace, id_objednavka) VALUES (1, 1);
INSERT INTO rezervace_objednavka(id_rezervace, id_objednavka) VALUES (2, 2);
INSERT INTO rezervace_objednavka(id_rezervace, id_objednavka) VALUES (4, 4);

INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (1, 1);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (1, 1);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (2, 1);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (2, 2);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (2, 2);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (3, 3);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (4, 3);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (4, 4);

INSERT INTO zaznam_platby(cislo_pobocky, cislo_pokladny, datum, rezim_platby, FIK, DIC, id_uzivatele, celkova_cena) VALUES (11, 3, TO_DATE('2010-01-03 19:34:34', 'yyyy-mm-dd hh24:mi:ss'), 'bezny', '3a6s8652sd3a6s8652sd3a6s8652sd3a6s8652s', 'CZ1234567890', 1, 400);
INSERT INTO zaznam_platby(cislo_pobocky, cislo_pokladny, datum, rezim_platby, FIK, DIC, id_uzivatele, celkova_cena) VALUES (11, 3, TO_DATE('2010-05-01 18:13:24', 'yyyy-mm-dd hh24:mi:ss'), 'bezny', 'poldv35214poldv35214poldv35214poldv3521', 'CZ1234567890', 1, 260);
INSERT INTO zaznam_platby(cislo_pobocky, cislo_pokladny, datum, FIK, DIC, id_uzivatele, celkova_cena) VALUES (11, 1, TO_DATE('2020-04-05 19:27:03', 'YYYY-MM-DD HH24:MI:SS'), '33344kllk-45ff-543-dfggd-44243234366-09', 'CZ1234567890', 4, 145);
INSERT INTO zaznam_platby(cislo_pobocky, cislo_pokladny, datum, FIK, DIC, id_uzivatele, celkova_cena) VALUES (11, 1, TO_DATE('2020-03-21 10:33:49', 'YYYY-MM-DD HH24:MI:SS'), '12345kadf-45ff-543-xcjkl-44243234311-09', 'CZ1234567890', 5, 90);

INSERT INTO objednavka_zaznam(id_objednavka, id_zaznam) VALUES (1, 1);
INSERT INTO objednavka_zaznam(id_objednavka, id_zaznam) VALUES (2, 2);
INSERT INTO objednavka_zaznam(id_objednavka, id_zaznam) VALUES (3, 4);
INSERT INTO objednavka_zaznam(id_objednavka, id_zaznam) VALUES (4, 3);

--ukazka triggeru (jmeno spatne zadano)
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2010-01-03 18:00', 'yyyy-mm-dd hh24:mi'), 'As', 'sfsgs@sd.cz', '+420123456789', 1, 5);
--ukazka triggeru (spatny format e-mailu)
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2010-01-03 18:00', 'yyyy-mm-dd hh24:mi'), 'Jirka Starman', 'sfsgs.cz', '+420123456789', 2, 6);
--ukazka triggeru (spatny format tel. cisla)
INSERT INTO rezervace(datum, jmeno, email, tel_cislo, id_uzivatele, pocet_osob) VALUES (TO_DATE('2010-01-03 18:00', 'yyyy-mm-dd hh24:mi'), 'Patrik Vasek', 'sfsgs@sd.cz', '123456789', 3, 8);
SELECT * FROM rezervace;

-- zkouska procedury
Select * from UZIVATEL;
Select * from OBJEDNAVKA;
begin
  proc_delete_user(1);
end;
/
Select * from UZIVATEL;
Select * from OBJEDNAVKA;

-- zkouska trigeru, ktery vola proceduru
SELECT * FROM objednavka;
SELECT * FROM objednavka_polozka;
INSERT INTO objednavka(cas_vytvoreni, id_stolu, id_uzivatele) VALUES (TO_DATE('2020-04-10 19:24:21', 'yyyy-mm-dd hh24:mi:ss'), 1, 2);
INSERT INTO objednavka_polozka(id_objednavky, id_polozky) VALUES (5, 1);
SELECT * FROM objednavka;
SELECT * FROM objednavka_polozka;

-- pre kazdeho uzivatela vypise sumu cien zaznamov platby, ktore vytvoril za urcite obdobie
DROP INDEX zaznam_datum_idx;
EXPLAIN PLAN FOR
	SELECT  u.jmeno, u.prijmeni, SUM(zp.celkova_cena)
	FROM uzivatel u, zaznam_platby zp
	WHERE u.id = zp.id_uzivatele and zp.datum BETWEEN  TO_DATE('2020-04-01','yyyy-mm-dd') AND TO_DATE('2020-04-30','yyyy-mm-dd')
	GROUP BY u.jmeno, u.prijmeni;
SELECT * FROM TABLE (DBMS_XPLAN.DISPLAY());

CREATE INDEX zaznam_datum_idx ON zaznam_platby (datum);

EXPLAIN PLAN FOR
	SELECT  u.jmeno, u.prijmeni, SUM(zp.celkova_cena)
	FROM uzivatel u, zaznam_platby zp
	WHERE u.id = zp.id_uzivatele and zp.datum BETWEEN  TO_DATE('2020-04-01','yyyy-mm-dd') AND TO_DATE('2020-04-30','yyyy-mm-dd')
	GROUP BY u.jmeno, u.prijmeni;
SELECT * FROM TABLE (DBMS_XPLAN.DISPLAY());

GRANT ALL ON ALERGEN TO XMORES01;
GRANT ALL ON ALERGEN_SUROVINA TO XMORES01;
GRANT ALL ON SUROVINA TO XMORES01;
GRANT ALL ON POLOZKA_MENU_SUROVINA TO XMORES01;
GRANT ALL ON POLOZKA_MENU TO XMORES01;
GRANT ALL ON OBJEDNAVKA_POLOZKA TO XMORES01;
GRANT SELECT, INSERT, UPDATE ON OBJEDNAVKA TO XMORES01;
GRANT ALL ON REZERVACE_OBJEDNAVKA TO XMORES01;
GRANT ALL ON OBJEDNAVKA_ZAZNAM TO XMORES01;
GRANT ALL ON UZIVATEL TO XMORES01;
GRANT ALL ON REZERVACE_STUL TO XMORES01;
GRANT SELECT, INSERT, UPDATE ON STUL TO XMORES01;
GRANT SELECT, INSERT, UPDATE ON ZAZNAM_PLATBY TO XMORES01;
GRANT ALL ON REZERVACE TO XMORES01;
GRANT EXECUTE ON proc_delete_user TO XMORES01;
GRANT EXECUTE ON set_objednavka_cena to XMORES01;
GRANT ALL on zamestnanci_uzivatele TO XMORES01;

-- view zobrazi nazov polozky a alergenu, ktory obsahuje
DROP MATERIALIZED VIEW alergeny_polozky;
CREATE MATERIALIZED VIEW alergeny_polozky
BUILD IMMEDIATE
REFRESH ON COMMIT
AS
    SELECT xzakji02.polozka_menu.nazev as Polozka, xzakji02.alergen.nazev as Alergen
    FROM xzakji02.surovina, xzakji02.alergen, xzakji02.alergen_surovina, xzakji02.polozka_menu, xzakji02.polozka_menu_surovina
    WHERE xzakji02.polozka_menu.id = xzakji02.polozka_menu_surovina.id_polozky and xzakji02.polozka_menu_surovina.id_suroviny = xzakji02.surovina.id and
    xzakji02.surovina.id = xzakji02.alergen_surovina.id_suroviny and xzakji02.alergen.id = xzakji02.alergen_surovina.id_alergenu;

GRANT ALL ON alergeny_polozky to XMORES01;

SELECT * FROM alergeny_polozky;

INSERT INTO polozka_menu(nazev, cena, hmotnost_jidla) VALUES ('Rybí polévka', 80, 200);
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Losos', '5 kg');
INSERT INTO surovina(nazev, mnozstvi_sklad) VALUES ('Těstoviny', '10 kg');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (5, 6, '0.25 kg');
INSERT INTO polozka_menu_surovina(id_polozky, id_suroviny, mnozstvi) VALUES (5, 7, '0.25 kg');
INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (4, 6);
INSERT INTO alergen_surovina(id_alergenu, id_suroviny) VALUES (1, 7);
COMMIT;

SELECT * FROM alergeny_polozky;