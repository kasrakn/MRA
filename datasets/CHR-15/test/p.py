import pandas as pd
import numpy as np

report = pd.read_csv("./test.csv")
ds = pd.read_csv("./Marshfield.csv")

ds_start = ds_end = rp_start = rp_end  = jacard = jacard = counter = avgjacard =  recal = avgrecal = 0




Result = pd.DataFrame()










for i in range(len(ds)):
    jacard = 0
    counter = 0
    recal = 0
    ds_start = ds.iloc[i,0]
    ds_end = ds.iloc[i,1]

    for j in range(len(report)):


        rp_start = report.iloc[j,0]
        rp_end = report.iloc[j,1]

        if ds_start <= rp_end and ds_end >= rp_start:

            counter = counter + 1
            list = (ds_start , ds_end , rp_start , rp_end)
            smallest = min(list)
            bigest = max(list)
            union = (bigest - smallest) + 1
            intersection = ( min( rp_end, ds_end ) - max(ds_start,rp_start) ) + 1
            jacard = jacard + float(intersection) / union

            if(recal < (float(intersection) / union) ):
                recal = float(intersection) / union




    if(counter > 0 ):
        marker = ds.iloc[i,2]
        Result = Result.append(pd.DataFrame({"P":float(jacard)/counter ,"R": recal,"Marker": marker, "#TRs": counter},index=[0]))
    elif(counter == 0):
        marker = ds.iloc[i,2]
        Result = Result.append(pd.DataFrame({"P": 0 ,"R": 0,"Marker": marker, "#TRs": 0},index=[0]))

    counter = 0
    recal = 0





Result.reset_index(inplace = True)
Result.drop('index',axis=1,inplace=True)
Result.to_csv("Result.csv")
