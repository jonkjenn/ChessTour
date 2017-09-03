--
-- File generated with SQLiteStudio v3.1.1 on Fri Sep 1 09:46:38 2017
--
-- Text encoding used: UTF-8
--
PRAGMA foreign_keys = off;
BEGIN TRANSACTION;

-- Table: Match
DROP TABLE IF EXISTS [Match];

CREATE TABLE [Match] (
    ResultWhite         DOUBLE,
    ResultBlack         DOUBLE,
    White               STRING,
    Black               STRING,
    WhiteFide           INTEGER NOT NULL
                                REFERENCES Players (FideId),
    BlackFide           INTEGER NOT NULL
                                REFERENCES Players (FideId),
    Priority            INTEGER,
    GamePosition        STRING,
    FullGameRKey        STRING,
    CurrentFEN          STRING,
    EarlierFEN          STRING,
    LastMoves           STRING,
    DeepPrevious        DOUBLE,
    DeepCurrent         DOUBLE,
    DeepDepth           INTEGER,
    DeepChange          DOUBLE,
    DeepMovePrevious    STRING,
    DeepMoveCurrent     STRING,
    RoundId             INTEGER REFERENCES Round (Id) ON DELETE CASCADE
                                NOT NULL,
    Number              INT     NOT NULL,
    GameNumber          INT     NOT NULL,
    Status              STRING,
    EngineScore         INT,
    EngineMate          INT,
    Id                  INTEGER PRIMARY KEY AUTOINCREMENT
                                UNIQUE
                                NOT NULL,
    PreviousEngineScore INT,
    PreviousEngineMate  INT
);


-- Table: Players
DROP TABLE IF EXISTS Players;

CREATE TABLE Players (
    Name   TEXT   NOT NULL,
    FideId STRING NOT NULL
                  UNIQUE
);


-- Table: Round
DROP TABLE IF EXISTS Round;

CREATE TABLE Round (
    Number       INT     NOT NULL,
    Date         STRING,
    Description  STRING,
    TournamentId INTEGER REFERENCES Tournament (Id) ON DELETE CASCADE,
    Id           INTEGER PRIMARY KEY AUTOINCREMENT,
    StartDate    STRING
);


-- Table: Tournament
DROP TABLE IF EXISTS Tournament;

CREATE TABLE Tournament (
    Name              STRING  NOT NULL
                              UNIQUE,
    OriginalOrder     INT,
    Status            STRING,
    EventType         STRING,
    CurrentShownRound INT     DEFAULT ( -1),
    Id                INTEGER PRIMARY KEY AUTOINCREMENT
                              NOT NULL,
    Subscribed        BOOLEAN,
    LastUpdated       STRING,
    Title             STRING,
    StartDate         STRING,
    CurrentRound      INT
);


COMMIT TRANSACTION;
PRAGMA foreign_keys = on;
