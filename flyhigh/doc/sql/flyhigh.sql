-- phpMyAdmin SQL Dump
-- version 2.6.1-rc1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Feb 15, 2005 at 10:54 PM
-- Server version: 4.0.22
-- PHP Version: 4.3.10-2
-- 
-- Database: `kflychart`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `Flights`
-- 

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
  PRIMARY KEY  (`Number`)
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
  PRIMARY KEY  (`Name`)
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
