-- phpMyAdmin SQL Dump
-- version 2.6.1-rc1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Mar 02, 2005 at 11:14 PM
-- Server version: 4.0.22
-- PHP Version: 4.3.10-2
-- 
-- Database: `flyhigh`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `Flights`
-- 

CREATE DATABASE flyhigh;
USE flyhigh;

CREATE TABLE `Flights` (
  `Number` int(11) NOT NULL default '0',
  `Date` date NOT NULL default '0000-00-00',
  `Time` time NOT NULL default '00:00:00',
  `Glider` varchar(16) NOT NULL default '0',
  `StartPt` varchar(16) NOT NULL default '0',
  `LandPt` varchar(16) NOT NULL default '0',
  `Duration` int(11) NOT NULL default '0',
  `Distance` int(11) default '0',
  `Comment` varchar(200) default NULL,
  `IGCFile` mediumblob,
  PRIMARY KEY  (`Number`),
	constraint FK_fStartPt foreign key(StartPt) references WayPoints(Name),
	constraint FK_fLandPt foreign key(LandPt) references WayPoints(Name),
	constraint FK_fGlider foreign key(Glider) references Gliders(Model)
) TYPE=InnoDB;

-- --------------------------------------------------------

-- 
-- Table structure for table `Gliders`
-- 

CREATE TABLE `Gliders` (
  `Manufacturer` varchar(16) NOT NULL default '',
  `Model` varchar(16) NOT NULL default '',
  PRIMARY KEY  (`Model`),
  UNIQUE KEY `byGlider` (`Manufacturer`,`Model`)
) TYPE=InnoDB;

-- --------------------------------------------------------

-- 
-- Table structure for table `LastModified`
-- 

CREATE TABLE `LastModified` (
  `Name` varchar(16) NOT NULL default '',
  `Time` datetime default NULL,
  PRIMARY KEY  (`Name`)
) TYPE=InnoDB;

-- --------------------------------------------------------

-- 
-- Table structure for table `Routes`
-- 

CREATE TABLE `Routes` (
  `Name` varchar(16) NOT NULL default '',
  `WayPoint0` varchar(16) default NULL,
  `WayPoint1` varchar(16) default NULL,
  `WayPoint2` varchar(16) default NULL,
  `WayPoint3` varchar(16) default NULL,
  `WayPoint4` varchar(16) default NULL,
  `WayPoint5` varchar(16) default NULL,
  `WayPoint6` varchar(16) default NULL,
  `WayPoint7` varchar(16) default NULL,
  `WayPoint8` varchar(16) default NULL,
  `WayPoint9` varchar(16) default NULL,
  `WayPoint10` varchar(16) default NULL,
  `WayPoint11` varchar(16) default NULL,
  `WayPoint12` varchar(16) default NULL,
  `WayPoint13` varchar(16) default NULL,
  `WayPoint14` varchar(16) default NULL,
  `WayPoint15` varchar(16) default NULL,
  `WayPoint16` varchar(16) default NULL,
  `WayPoint17` varchar(16) default NULL,
  `WayPoint18` varchar(16) default NULL,
  `WayPoint19` varchar(16) default NULL,
  `WayPoint20` varchar(16) default NULL,
  `WayPoint21` varchar(16) default NULL,
  `WayPoint22` varchar(16) default NULL,
  `WayPoint23` varchar(16) default NULL,
  `WayPoint24` varchar(16) default NULL,
  `WayPoint25` varchar(16) default NULL,
  `WayPoint26` varchar(16) default NULL,
  `WayPoint27` varchar(16) default NULL,
  `WayPoint28` varchar(16) default NULL,
  `WayPoint29` varchar(16) default NULL,
  PRIMARY KEY  (`Name`),
	constraint FK_fWayPoint0 foreign key(WayPoint0) references WayPoints(Name),
	constraint FK_fWayPoint1 foreign key(WayPoint1) references WayPoints(Name),
	constraint FK_fWayPoint2 foreign key(WayPoint2) references WayPoints(Name),
	constraint FK_fWayPoint3 foreign key(WayPoint3) references WayPoints(Name),
	constraint FK_fWayPoint4 foreign key(WayPoint4) references WayPoints(Name),
	constraint FK_fWayPoint5 foreign key(WayPoint5) references WayPoints(Name),
	constraint FK_fWayPoint6 foreign key(WayPoint6) references WayPoints(Name),
	constraint FK_fWayPoint7 foreign key(WayPoint7) references WayPoints(Name),
	constraint FK_fWayPoint8 foreign key(WayPoint8) references WayPoints(Name),
	constraint FK_fWayPoint9 foreign key(WayPoint9) references WayPoints(Name),
	constraint FK_fWayPoint10 foreign key(WayPoint10) references WayPoints(Name),
	constraint FK_fWayPoint11 foreign key(WayPoint11) references WayPoints(Name),
	constraint FK_fWayPoint12 foreign key(WayPoint12) references WayPoints(Name),
	constraint FK_fWayPoint13 foreign key(WayPoint13) references WayPoints(Name),
	constraint FK_fWayPoint14 foreign key(WayPoint14) references WayPoints(Name),
	constraint FK_fWayPoint15 foreign key(WayPoint15) references WayPoints(Name),
	constraint FK_fWayPoint16 foreign key(WayPoint16) references WayPoints(Name),
	constraint FK_fWayPoint17 foreign key(WayPoint17) references WayPoints(Name),
	constraint FK_fWayPoint18 foreign key(WayPoint18) references WayPoints(Name),
	constraint FK_fWayPoint19 foreign key(WayPoint19) references WayPoints(Name),
	constraint FK_fWayPoint20 foreign key(WayPoint20) references WayPoints(Name),
	constraint FK_fWayPoint21 foreign key(WayPoint21) references WayPoints(Name),
	constraint FK_fWayPoint22 foreign key(WayPoint22) references WayPoints(Name),
	constraint FK_fWayPoint23 foreign key(WayPoint23) references WayPoints(Name),
	constraint FK_fWayPoint24 foreign key(WayPoint24) references WayPoints(Name),
	constraint FK_fWayPoint25 foreign key(WayPoint25) references WayPoints(Name),
	constraint FK_fWayPoint26 foreign key(WayPoint26) references WayPoints(Name),
	constraint FK_fWayPoint27 foreign key(WayPoint27) references WayPoints(Name),
	constraint FK_fWayPoint28 foreign key(WayPoint28) references WayPoints(Name),
	constraint FK_fWayPoint29 foreign key(WayPoint29) references WayPoints(Name)
) TYPE=InnoDB;

-- --------------------------------------------------------

-- 
-- Table structure for table `Servicing`
-- 

CREATE TABLE `Servicing` (
  `Id` int(11) NOT NULL default '0',
  `Glider` varchar(16) NOT NULL default '',
  `Date` date NOT NULL default '0000-00-00',
  `Responsibility` varchar(16) NOT NULL default '',
  `Comment` varchar(200) NOT NULL default '',
  PRIMARY KEY  (`Id`),
	constraint FK_fGlider foreign key(Glider) references Gliders(Model)
) TYPE=InnoDB;

-- --------------------------------------------------------

-- 
-- Table structure for table `WayPoints`
-- 

CREATE TABLE `WayPoints` (
  `Name` varchar(16) NOT NULL default '',
  `Longitude` float default NULL,
  `Latitude` float default NULL,
  `Altitude` int(11) NOT NULL default '0',
  `Description` varchar(200) NOT NULL default '',
  PRIMARY KEY  (`Name`)
) TYPE=InnoDB;
