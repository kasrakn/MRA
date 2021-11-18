# Precision-Recall experiment datasets
## How to calculate the precision and recall value?

Well, this is almost a lengthy process. To calculate the precision and recall values of a specific dataset, you should first run the Python code for each chromosome with its related dataset. The precision and recall of each chromosome are equal to the average value of the precision and recall of each tandem repeat (rows in the output CSV file). To calculate the precision and recall values for a specific dataset, you need to calculate the average precision and recall of chromosomes.

For example, to calculate the precision and recall for the Promoters dataset, run the Python script for chromosomes 1,2, ...X, Y with the presented datasets in this directory, and then calculate the average precision and recall values for each chromosome. In the end, calculate the average value of that average values which got in the previous step.

The math formula presented in the Dot2dot paper for calculating the precision and recall values:
<br><br>
$$
\sigma_P(T, R)=\frac{1}{n} \left({\sum_{i=1}^{n} }{\frac{\sum_{x \in R(t_i)} jac(x, t_i)}{|R(t_i)|}} \right)
$$

$$
\sigma_R(T, R)=\frac{1}{n} \left({\sum_{i=1}^{n}\mathop{}_{\mkern-5mu x\in R(t_i)} \max jac(x, t_i) } \right)
$$