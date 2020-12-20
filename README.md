# Soccer Result Simulator

<img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/appIcon.ico" align="right"
     alt="Logo of Soccer Result Simulator" width="128" height="128">
     
Soccer Result Simulator is supposed to be tool that simulates the results of football matches **by one click**.  

## The results(goals for each team) are generated according to the concept which is explained below  
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



## A little guide and a small presentation of the functions and possibilities of this tool  
**Dialog of table and match results**  
<p align="center">
  <img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/Neuer%20Ordner/example1.png" alt="Example 1" width="738">
</p>
