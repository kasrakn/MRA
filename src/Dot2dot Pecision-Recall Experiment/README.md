# Precision-Recall Experience 
To measure the accuracy level of the MPA algorithm, we used the precision-recall method which presented in the <i>[Dot2dot](https://academic.oup.com/bioinformatics/article/35/6/914/5085378)</i> paper. Since MPA is an algorithm for finding perfect tandem repeats, we extracted datasets for perfect tandem from the original datasets used in Dot2dot. This code is implemented to compare our output results with the datasets.
<br>
<br>

## **How to run the code?**
To run this code, if Python is not installed, you can install it by visiting [their website](https://www.python.org). Next, to install the dependency libraries, you can use the requirements.txt file located in the current directory. One way of installing these dependencies is to use the [pip](https://pypi.org/) command.
```shell
pip install -r requirements.txt
```
After installing the dependencies, you can run the code using `python` command
```shell
python main.py -i PATH_TO_INPUT -d PATH_TO_DATASET -o PATH_TO_OUTPUT_DIR
```


### **Parameters**:
| Flag            | Defenition                                                                                                               |
|-----------------|--------------------------------------------------------------------------------------------------------------------------|
| -i or --input   | Path to a output file of the algorithm which has been created usign the -D command in the original code of the algorithm |
| -d or --dataset | Path to the dataset.                                                                                                     |
| -o or --output  | Path the directory of the output file.                                                                                   |