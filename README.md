# Soccer Result Simulator

<img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/appIcon.ico" align="right"
     alt="Logo of Soccer Result Simulator" width="128" height="128">
     
Soccer Result Simulator is supposed to be tool that simulates the results of football matches **by one click**.

## The results(goals for each team) are generated according to the concept which is explained below
Firstly, this is not the final concept. The long-term goal is to generate the results with the help of **machine-learning/deep-learning**
based on a concept which is discriped in the book **"Der perfekte Tipp. Statistik des Fußballspiels"(The perfect tip. Statistics of the football match) by Andreas Heuer.**

At the moment, there are three values for each team in the database. Offense, defense and teamability. The values are between 20 and 99.
99 is the best and 20 is the weakest value. The values are not used for calculation. It´s more user friendly. 
This values will be converted in the so-called mean-value(for example 99 means 4,9...) which is used by the "Poisson-distribution".
This distribution generates a random number.

After conversion we can calculate this values with;
mean value for home team = offense(home team) * defense(guest team) + home bonus(0,41)
and mean value for guest team = offense(guest team) * defense(away team).
We hand over this values to the Poisson-distribution, which generates a random number(number of goals) for each team.

**The effect of the teamability is as follow:**
The teamability value is placed on a Bernoulli-distribution(propability of return 0 or 1) and is interpreted as a probability of scoring one goal more and getting one goal less.
The higher the value, the higher is the probability of the team to score one goal and getting one goal less. **So you can say the teamability improves both ofensive and defensive.**

 Next, I will implement how the results of the last 5 Games will influence the current match results.

## A little guide and a small presentation of the functions and possibilities
**Dialog of table and match results**
<p align="center">
  <img src="https://github.com/M87-virgo/soccer_result_smltr/blob/main/misc_icons/Neuer%20Ordner/example1.png" alt="Example 1" width="738">
</p>
