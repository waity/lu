create table Sequence (
  sequenceID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  listLength INTEGER NOT NULL,
  requestLength INTEGER NOT NULL,
  sequence VARCHAR(20)
);

CREATE UNIQUE INDEX uq_sequence
  ON Sequence(listLength, requestLength, sequence);

create table Cost (
  sequenceID INTEGER NOT NULL,
  cost SMALLINT UNSIGNED NOT NULL,
  algorithmID INTEGER NOT NULL,
  PRIMARY KEY(sequenceID, algorithmID),
  Foreign Key(algorithmID) references Algorithm(algorithmID),
  Foreign Key(sequenceID) references Sequence(sequenceID)
);

create table Algorithm (
  algorithmID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  name VARCHAR(100) UNIQUE
);