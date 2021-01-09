# Soccer Result Simulator

<br/>
<div align="center">
     <br />
     <img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/appIcon.ico" alt="Logo of Soccer Result Simulator" width="128" height="128"/>
</div>     


<div align="center">
     
Soccer Result Simulator is supposed to be tool that simulates the results of football matches **by one click**.

</div>

<div align="center">

**Soccer Result Simulator 1.2.1 has been released.**
**You can download it [here](https://github.com/M87-virgo/soccer_result_smltr/raw/main/soccer_result_simltr/installer/srsSetup.exe).**  
     
</div>

<img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/Neuer%20Ordner/Soccer%20Result%20Simulator.jpg" alt="Example 1" width="1024">

- [**Motivation**](#motivation)
- [**Getting Started**](#getting-started)
- [**Invitation for participation**](#participation)
- [**Guide and Features**](#features)
- [**Concept and Technical Background**](#concept)
- [**Preview**](#preview)
- [**Contributing**](#contributing)
- [**Feedback**](#feedback)
- [**License**](#license)

## Motivation
Coming soon...

## Getting Started
- [Download](https://github.com/M87-virgo/soccer_result_smltr/raw/main/soccer_result_simltr/installer/srsSetup.exe) the Installer-File named srsSetup.exe
- Install the tool to a directory of your choice
- Execute soccer_result_simltr.exe to start the programm
- Read the [guide](#features) to explore what you can do and how you can use the programm

## Participation
Coming soon...

## Features
**A little guide and a small presentation of the functions and possibilities of this tool**   
**Dialog of table and match results**  
More coming soon...
  
  **Feature-List resp. step-by-step Guide**
- Choose between Nationalteams-Mode and Clubs-Mode
- Nationalteams are structured in Continents -> Sub-Continents.
- Choose single teams, All teams from a Sub-Continent or from a whole Continent
- Clubs are just structured in leagues, so you can choose single clubs or all clubs from a league
- Set the round number(max. 8). It means rounds of matches and re-matches. For example if you choose 3, you got matches 1; re-matches 1; matches 2. Max is 8. So, 4 rounds of matches and 4 rounds of re-matches, alternately
- Optional you can add teams randomly
- The play level values are shown below the Participants-List by selecting a team <img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/Neuer%20Ordner/play_level_example.jpg" alt="playlevel" width="260" align="left">
- Simulate all the results and show the end-table or simulate only the first Match-Day by one click
- With the arrow-buttons you can navigate forward and back through the match days and the associated table
- Display the home table and away table
- Select single matches
- Edit the results. By pressing the enter-key the table will be updated
- Save and Load Team-Lists
- Save and Load Results and associated Tables
- Load other Databases
- Edit the Database with the help of the Database-Editor <img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/Neuer%20Ordner/DB_Editor.jpg" alt="database_editor" width="460" align="left">

## Concept  
Firstly, this is not the final concept. The long-term goal is to generate the results with the help of **machine-learning/deep-learning**  
based on a concept which is discriped in the book **"Der perfekte Tipp. Statistik des Fußballspiels"(The perfect tip. Statistics of the football match) by Andreas Heuer.**  
**I invite everyone to participate in this project. Help with machine-learning is very welcome.**

At the moment, the play level of each team is modeled by three values in the database: offensive, defensive and teamability. 
The values are between 20 and 99.  
99 is the best and 20 is the weakest value. These values are not used for calculation. It´s just more user friendly.  
These values are converted into the so-called "mean-value"(for example 99 is equivalent to about 4,9...; according to a conversion equation)  
which is used by the "Poisson-distribution". That´s why it have to be a real-number(single digit comma number).    
This distribution generates a random number.  

After conversion we can calculate this values with;  
mean value for home team = (offensive(home team) * defensive(guest team) ) + home bonus(0,41)  
and mean value for guest team = offensive(guest team) * defensive(away team).  
We pass this value to the Poisson-distribution, which generates a random number(number of goals) for each team.  
**Example**  
Team A value = 1,238 (about one goal is the most probable case)  
Team B value = 3,017 (about 3 goals is the most probable case)  
To genarate random numbers means that a result like 0:2 or 3:2 etc. is possible too.  

**The effect of the teamability is as follow:**  
The teamability value is passed to the Bernoulli-distribution(propability of return 0 or 1) and is interpreted as a   
probability of scoring one goal more and getting one goal less.  
The higher the value, the higher is the probability of the team to score one goal and getting one goal less.   
**So you can say the teamability improves both offensive and defensive.**  

**Next, I will implement how the results of the last 5 Games will influence the current match results.**   

**How do I estimate the values of the teams?**  
The values are really just rough estimates.  
For clubs I look at the tables from the current season or/and the end table from the last season.   
For Nationalteams I look at the World-Ranking table.   
**A little example for Atalanta Bergamo:**  
This team came third in season 2019/2020 and scores 98 goals in 38 games. This suggests a strong offensive.  
So it can be a value about 90. Bergamo got 48 goals against. So the defensive is a little weak. Maybe a value about 75 is ok.  
Bergamo is not a team with very famous players. So you can say the teamability of this team must be high.   
They surprised with third place in league and the success in the CL. Maybe a value about 90 is good.  

It would go beyond the scope if I discripe it in all details.  
But, everyone is welcome to determine his own values. You can edit the values from the database using the database-editor of my tool.  

## Preview
Coming soon...

## Contributing
Coming soon...

## Feedback
beyer341@onlinehome.de

## License  
<p>This program uses Qt version 5.12.9.</p>
<p>Qt is a C++ toolkit for cross-platform application development.</p><p>Qt provides single-source portability across all major desktop operating systems. It is also available for embedded Linux and other embedded and mobile operating systems.</p><p>Qt is available under multiple licensing options designed to accommodate the needs of our various users.</p><p>Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do not want to share any source code with third parties or otherwise cannot comply with the terms of GNU (L)GPL.</p><p>Qt licensed under GNU (L)GPL is appropriate for the development of Qt&nbsp;applications provided you can comply with the terms and conditions of the respective licenses.</p><p>Please see <a href="http://qt.io/licensing/">qt.io/licensing</a> for an overview of Qt licensing.</p><p>Copyright (C) 2020 The Qt Company Ltd and other contributors.</p><p>Qt and the Qt logo are trademarks of The Qt Company Ltd.</p><p>Qt is The Qt Company Ltd product developed as an open source project. See <a href="http://qt.io/">qt.io</a> for more information.</p>
