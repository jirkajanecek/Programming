﻿CREATE TABLE [dbo].[owners]
(
	[Id] INT NOT NULL PRIMARY KEY IDENTITY(1,1),
	[BirthNumber] VARCHAR(10) NOT NULL,
	[Firstname] VARCHAR(20) NOT NULL,
	[Surname] VARCHAR(50) NOT NULL
)
