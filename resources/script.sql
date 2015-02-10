CREATE DATABASE hrtdb;
USE hrtdb;
DROP TABLE `projects`;
CREATE TABLE `projects` (
	projectId	INT(4) AUTO_INCREMENT,
	projectName	VARCHAR(20),
	coreInputFile VARCHAR(255),
	fortPath	VARCHAR(255),
	iniPath		VARCHAR(255),
	creationDate	TIMESTAMP NOT NULL,
	PRIMARY KEY (projectId),
	UNIQUE KEY `name_ix` (`projectName`));

CREATE TABLE `sourceData` (
	fileId	INT(4) AUTO_INCREMENT,
	projectName	VARCHAR(20),
	fileType	varchar(20),
	fileName	VARCHAR(255),
	PRIMARY KEY (fileId));

CREATE TABLE `ports` (
	fileId	INT(4),
	portType	VARCHAR(20),
	portName	VARCHAR(20));

CREATE TABLE `unitNets` (
	unitId	INT(4) AUTO_INCREMENT,
	fileId	INT(4) DEFAULT 0,
	melUnitName	VARCHAR(20),
	fortUnitName	VARCHAR(20),
	melUnitType	VARCHAR(20),
	macroUnitName	VARCHAR(20),
	isPlaced	BOOLEAN DEFAULT 0,
	PRIMARY KEY (unitId),
	UNIQUE KEY `unitIx` (`fileId`,`melUnitName`,`fortUnitName`));

CREATE TABLE `unitPorts` (
	unitId	INT(4),
	portType	VARCHAR(20),sourcedata
	portName	VARCHAR(20));

CREATE TABLE `route` (
	routeId	INT(4),
	routeName	VARCHAR(20),
	direction	VARCHAR(20),
	begin	VARCHAR(20),
	end		VARCHAR(20),
	beginPoint	INT(4),
	endPoint	INT(4),
	routeChannel	INT(4),
	timestamp	DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
	isHidden	BOOLEAN,
	PRIMARY KEY (routeId));

CREATE TABLE `unitPlacement` (
	unitId	INT(4),
	unitRow	INT(4),
	unitColumn	INT(4),
	created	TIMESTAMP	NOT NULL,
	isHidden	BOOLEAN DEFAULT 0,
	UNIQUE KEY `placement_ix` (unitId, isHidden));

SHOW TABLES;
SELECT * FROM unitNets;
SELECT * FROM unitPorts;
SELECT * FROM unitPlacement;
SELECT * FROM `sourceData`;
SELECT * FROM `projects`;
TRUNCATE TABLE `projects`; 
TRUNCATE TABLE `sourceData`;
TRUNCATE TABLE `unitNets`;
TRUNCATE TABLE `unitPorts`;
TRUNCATE TABLE `unitPlacement`;
DROP TABLE unitPlacement;
SELECT melUnitName, fortUnitName FROM unitNets WHERE isPlaced IS NOT NULL AND isPlaced != 1;
DROP TABLE `unitPlacement`;

DESCRIBE `unitPlacement`;
SELECT unitNets.melUnitName, unitPlacement.unitRow, unitPlacement.unitColumn, unitPlacement.unitId, unitNets.fileId FROM sourceData, unitNets, unitPlacement
	WHERE unitPlacement.unitId = unitNets.unitId AND unitNets.fileId = sourceData.fileId AND sourceData.projectName = "HV154k1";

SELECT A.isPlaced, a.melUnitName, A.fileId FROM unitNets A, sourceData B, projects C
	WHERE A.fileId = B.fileId AND C.projectName = "HV154k0" 
	AND C.coreInputFile = B.fileName AND A.melUnitName = "DD1" 
	AND C.projectName = B.projectName;

SELECT COUNT(*) FROM unitNets A, sourceData B, projects C
	WHERE A.fileId = B.fileId AND C.projectName = "55" 
	AND A.melUnitName = "DD1" AND A.macroUnitName
	AND C.projectName = B.projectName;


UPDATE unitNets SET isPlaced = 1 
WHERE unitId = (
SELECT A.unitId FROM unitNets A, sourceData B, projects C 
	WHERE A.fileId = B.fileId AND C.projectName = "HV154k1" 
	AND C.coreInputFile = B.fileName AND A.melUnitName = "DD6" 
	AND C.projectName = B.projectName );

SELECT A.unitId FROM unitNets A, sourceData B, projects C 
                          WHERE A.fileId = B.fileId AND C.projectName = "HV154k1" 
                          AND C.coreInputFile = B.fileName AND A.melUnitName = "DD10" 
                          AND C.projectName = B.projectName;
UPDATE unitNets SET isPlaced = 1 WHERE unitId = 456;

SELECT unitNets.fortUnitName FROM unitNets, sourceData, projects WHERE 
                  unitNets.melUnitType = 1 AND unitNets.fileId = sourceData.fileId AND 
                  sourceData.projectName = projects.projectName AND sourceData.fileName = "1";

SELECT unitNets.melUnitName FROM unitNets, sourceData WHERE unitNets.melUnitType = 1 AND 
                                     unitNets.fileId = sourceData.fileId AND unitNets.fortUnitName = "SR17" AND sourceData.fileId = (
SELECT fileId FROM sourceData WHERE sourceData.projectName = "34534" AND sourceData.fileName = "C:/Users/Evgeniy/Desktop/Perl/MEL/Out/K11052.mel");
                                     
SELECT unitNets.melUnitName FROM unitNets, sourceData WHERE unitNets.melUnitType = 1 AND unitNets.fileId = sourceData.fileId AND unitNets.fortUnitName =  "BUF16"
AND sourceData.projectName =  "345342"  AND sourceData.fileName =  "C:/Users/Evgeniy/Desktop/Perl/MEL/Out/K11052.mel";

SELECT COUNT(*) FROM unitNets A, sourceData B, projects C 
                  WHERE A.fileId = B.fileId AND C.projectName = "55" 
                  AND A.melUnitName = "DD23" AND A.macroUnitName = "DD4"
                  AND C.projectName = B.projectName;

SELECT A.fortUnitName FROM unitNets A, sourceData B, projects C 
                      WHERE A.fileId = B.fileId AND C.projectName = "123" 
                      AND A.melUnitName = "DD55" AND A.macroUnitName = "0" 
                      AND C.projectName = B.projectName;

SELECT N.melUnitName, P.unitRow, P.unitColumn, P.unitId, N.macroUnitName FROM sourceData S, unitNets N, unitPlacement P, projects 
                  WHERE P.unitId = N.unitId AND N.fileId = S.fileId 
                  AND projects.projectName = "123" AND projects.projectName = S.projectName 
                  AND N.isPlaced = 1;