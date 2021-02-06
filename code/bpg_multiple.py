import sys
import os
import random
import pandas as pd
import math
import csv
import re

def print_dictionary(temp_dict):
    for i in temp_dict:
        print(i, temp_dict[i])

# Disable
def blockPrint():
    sys.stdout = open(os.devnull, 'w')

# Restore
def enablePrint():
    sys.stdout = sys.__stdout__

#handles division by zero
def safe_div(num, den):
    if(den == 0):
        return 0
    else:
        return num / den

#handles log of zero
def safe_log(val, base):
    if(val == 0):
        return 0
    else:
        return math.log(val, base)

#takes string interval and converts it to a list of list
def parse_interval_from_string(line, initial_trace_length):
    interval = []
    if(line == '{}'):
        return interval
    firstline = True
    for i in line.split(')')[:-1]:
        if(firstline):
            line = i[2:]
            firstline = False
        else:
            line = i[1:]
        line1 = line.split(':')
        first, second = float(line1[0]), float(line1[1])
        if(abs(initial_trace_length - second) <= 0.000001):
            second = initial_trace_length
            stop_computing = 1
        if(first < initial_trace_length):
            temp = []
            temp.append(first)
            temp.append(second)
            interval.append(temp)
    return interval

#reads from config file and stores the values
def read_from_config(config_file_name, signal_list, other_arguments):
    global influence_list_all
    i_s_regex = re.compile(r'^trace_\d*_start$')
    i_e_regex = re.compile(r'^trace_\d*_end$')
    
    trace_num = -1
    signal_read = 0
    influence_read = 0
    target_read = 0
    trace_length = 0
    fptr = open(config_file_name,'r+')
    strict = 0
    for row in fptr:
        if row == '\n':
            continue
        # Reading List of Signals
        if(signal_read == 1 and row != '\n'):
            line = row.strip().split(' ')
            if(line[0] == 'end'):
                signal_read = 0
                continue
            else:
                sig_name = line[0]
                sig_time = int(line[2])
                sig_value = int(line[1])
            temp_signal = []
            temp_signal.append(sig_name)
            temp_signal.append(sig_time)
            temp_signal.append(sig_value)
            signal_list.append(temp_signal)
            continue
        # Reading the Influence List
        if(influence_read == 1 and row != '\n'):
            line = row.strip().split(' ')
            if(line[0] == 'influence_list_end'):
                influence_read = 0
                continue
            
            elif(i_s_regex.match(line[0])):
                trace_num = int(line[0].split('_')[1]) - 1
                continue
            
            elif(i_e_regex.match(line[0])):
                trace_num = -1
                continue
            
            else:
                line = row.split(' ')
                bucket_number = int(line[0])
                string_interval = ''
                for i in range(1,len(line)):
                    string_interval = string_interval + line[i]
                    i+=1
                bucket_interval = parse_interval_from_string(string_interval,trace_length)
                current_influence_list_entry = []
                current_influence_list_entry.append(bucket_number)
                current_influence_list_entry.append(bucket_interval)
                influence_list_all[trace_num].append(current_influence_list_entry)
            continue
            
        if(target_read == 1 and row != '\n'):
            line = row.strip().split(' ')
            if(line[0] == 'target_end'):
                target_read = 0
                continue
            else:
                target_interval.append(parse_interval_from_string(row.strip(),trace_length))
            continue

        line = [i.strip() for i in row.strip().split('=')] #   row.strip().split('=').strip()
        if( line[0] == 'COMMENTS'):
            break
        if( line[0] == 'm'):
            m = line[1]
            continue
        if( line[0] == 'trace_length'):
            trace_length = float(line[1])
            continue
        if( line[0] == 'k'):
            k = line[1]
            continue
        if( line[0] == 'n'):
            n = line[1]
            continue
        if( line[0] == 'tmax'):
            tmax = line[1]
            continue
        if( line[0] == 'tmin'):
            tmin = line[1]
            continue
        if( line[0] == 'strict'):
            strict = line[1]
            continue
        if( line[0] == 'dataset_file'):
            if(line[0] == 'dataset_file'):
                csv_files_list = line[1].split(',')
                csv_files_list = [i.strip() for i in csv_files_list]
            for i in range(0, len(csv_files_list)):
                influence_list_all[i] = list()
            continue
        if( line[0] == 'bias'):
            bias = line[1]
            continue
        if( line[0] == 'verbosity'):
            if(line[1] == '1'):
                enablePrint()
            continue
        if( line[0] == 'start'):
            signal_read = 1
            continue
        if( line[0] == 'influence_list_start'):
            influence_read = 1
            continue
        if( line[0] == 'target_start'):
            target_read = 1
            continue

        if( line[0] == 'target'):
            if(len(line) == 2):
                target = line[1]
            if(len(line) == 3):
                target = line[1]+'= '+line[2]
            if(len(line) == 4):
                target = line[1]+' == '+line[3]
            target_predicate = target.split('"')[1]
            continue
    other_arguments.append(csv_files_list)
    other_arguments.append(m)
    other_arguments.append(k)
    other_arguments.append(tmax)
    other_arguments.append(tmin)
    other_arguments.append(n)
    other_arguments.append(trace_length)
    other_arguments.append(target_interval)
    other_arguments.append(bias)
    other_arguments.append(strict)

# computes complements the interval
def complement_interval(interval):
    c_interval = []
    if(len(interval) == 0):
        temp = []
        temp.append(0)
        temp.append(initial_trace_length)
        c_interval.append(temp)
        return c_interval
    # print 'finding complement for ',interval

    temp=[0.0,0.0]
    c_interval.append(temp)
    for i in range(0,len(interval)):
        c_interval[-1][1] = interval[i][0]
        temp = [interval[i][1],0.0]
        c_interval.append(temp)
    if(c_interval[-1][0] > initial_trace_length):
        print('anomaly found time value bigger than initial trace length')
    if(c_interval[-1][0] == initial_trace_length):
        del c_interval[-1]
    else:
        c_interval[-1][1]= initial_trace_length
    return c_interval

def sort_influence_list(influence_list):
    list_len = len(influence_list)
    if(list_len < 2):
        return
    i=0
    j=1
    while(i<list_len):
        j=i+1
        while(j<list_len):
            # print i,j
            if influence_list[i][0] < influence_list[j][0] :
                influence_list[i],influence_list[j] = influence_list[j],influence_list[i]
            j+=1
        i+=1

def get_min_bucket_value(influence_list):
    mbv = 0
    if(influence_list == []):
        print('influence list empty')
        return mbv
    mbv = n
    for i in range(0,len(influence_list)):
        if(influence_list[i][0] < mbv):
            mbv = influence_list[i][0]
    # for entry in influence_list:
    #   if (entry[0] < mbv):
    #       mbv = entry[0]
    return mbv

def fill_intersected_influence_list(n, influence_list):
    #print('inside fill intersected influence list')
    #print('printing influence list')
    #print(influence_list)
    intersected_influence_list = {}
    for i in range(n+1):
        intersected_influence_list[i] = []
    for entry in influence_list:
        bucket_number,bucket_interval=entry
        intersected_influence_list[bucket_number] = bucket_interval
    #print('printing intersected_influence_list')

    i = n-1
    while(i >= 0):
        if len(intersected_influence_list[i + 1])!=0:
            temp_interval = minkowski_sum(intersected_influence_list[i + 1],k)
            if(len(intersected_influence_list[i]) == 0):  
                intersected_influence_list[i]= temp_interval
            else:
                intersected_influence_list[i] = intersect_interval(temp_interval,intersected_influence_list[i])
        i-=1
    #print_dictionary(intersected_influence_list)
    intersected_influence_list_all.append(intersected_influence_list)

def merge_overlapping_intervals(interval):
    n_interval = []
    if(len(interval) == 0):
        return n_interval
    n_interval.append(interval[0])
    i=1
    index = 0
    interval_length = len(interval)
    while(i < interval_length):
        if(interval[i][0] <= n_interval[index][1] ):
            n_interval[index][1] = interval[i][1]
        else:
            n_interval.append(interval[i])
            index+=1
        i+=1
    return n_interval

def minkowski_sum(interval,k):
    n_interval = []
    stop_computing = 0
    for entry in interval:
        temp = []
        start = entry[0] + float(k)
        if(start >= initial_trace_length):
            end = initial_trace_length
            stop_computing = 1

        end = entry[1] + float(k)
        
        if( strict == 0):
            temp.append(entry[0])
        else:
            temp.append(start)
        
        if(end >= initial_trace_length):
            end = initial_trace_length
            stop_computing = 1
        temp.append(end)
        # temp.append(min(initial_trace_length ,entry[1] + float(k)))
        n_interval.append(temp)
        if (stop_computing == 1):
            break
    n_interval = merge_overlapping_intervals(n_interval)
    return n_interval

def minkowski_difference(interval,k):
    # print 'interval is',interval
    n_interval = []
    stop_computing = 0
    for entry in reversed(interval) :
        temp = []
        start = entry[0] - float(k)
        end = entry[1] - float(k)
        if( start < 0  ):
            temp.append(0)
            if( strict == 0 ):
                temp.append(entry[1])
            else:
                if( end < 0  ):
                    temp.append(0)
                else:
                    temp.append(end)
            stop_computing = 1
        else:
            temp.append(start)
            if( strict == 0 ):
                temp.append(entry[1])
            else:
                temp.append(end)
        n_interval.append(temp)
        if(stop_computing == 1):
            break

    n_interval.reverse()

    # print 'before ',interval
    # print 'after minkowski diff ',n_interval
    n_interval = merge_overlapping_intervals(n_interval)
    return n_interval

def eliminate_useless_signals(processed_signal_variable , signal_list):
    for entry in signal_list:
        # comparing minimum and maximum values
        if entry[1] == entry[2] :
            # that signal variable is marked as processed
            processed_signal_variable[entry[0]] = 1
            print('The signal variable ',entry[0],'has same minmax value and hence is marked as already processed')
    print('Eliminating useless signal completed')

def compute_min_max_of_signal_variable(csv_file, signal_list ,minmax_of_signal_variables):
    minmax_file_name = 'minimum_maximum_' + csv_file[:-4] +'.txt'
    temp_list = []
    print('minmax file name is ', minmax_file_name)
    my_file = os.path.exists('./' + minmax_file_name)
    if my_file:
        filereader = open(minmax_file_name,'r+')
        for row in filereader:
                if(row == '\n'):
                    continue
                line = row.strip().split(',')
                # print 'line ',line
                sig_name,min_val,max_val = line
                min_val = float(min_val)
                max_val = float(max_val)
                minmax_entry = []
                minmax_entry.append(sig_name)
                minmax_entry.append(min_val)
                minmax_entry.append(max_val)
                temp_list.append(minmax_entry)
        print('read minmax values from file')
        minmax_of_signal_variables.append(temp_list)
        return
    else:
        print('file not present')
    print('computing min and max for every signal variable')
    fptr = open(minmax_file_name,'w+')
    fptr.truncate()

    for entry in signal_list:
        col_val = entry[2]
        min_val=1000000
        max_val=-1000000
        first_line = 1
        with open(csv_file,'r') as csvfile:
            filereader = csv.reader(csvfile,delimiter=',')
            for row in filereader:
                if(first_line == 1):
                #first line is the signal variable name so we skip that.
                    first_line = 0
                    continue
                val=float(row[col_val-1])
                min_val = min(min_val,val)
                max_val = max(max_val,val)
        minmax_entry = []
        minmax_entry.append(entry[0])
        minmax_entry.append(min_val)
        minmax_entry.append(max_val)
        fptr.write(entry[0] + ',' +str(min_val)  + ',' +str(max_val) + '\n')
        temp_list.append(minmax_entry)
    print('computed min and max for every signal variable')
    minmax_of_signal_variables.append(temp_list)

def generate_pseudo_targets(pseudo_targets_all, n, k):
    for x in target_interval:
        i = 0
        pseudo_targets = {}
        while(i <= n):
            pseudo_targets[-1 * i] = minkowski_difference(x,i*k) 
            # pseudo_targets_error[-1 * i] = find_error_for_predicate_target( pseudo_targets[-1 * i])
            # print 'pseudo_targets error for (-',i,')',pseudo_targets_error[-1 *i]
            i+=1
        pseudo_targets_all.append(pseudo_targets)
              
def generate_pseudo_complement_targets(pseudo_complement_targets_all,n,k):
    for x in target_false_interval:
        i = 0
        pseudo_complement_targets = {}
        while(i <= n):
            pseudo_complement_targets[-1 * i] = minkowski_difference(x,i*k) 
            # pseudo_complement_targets_error [ -1 * i] = find_error_for_predicate_target (pseudo_complement_targets[-1 * i])
            # print 'pseudo_complement_targets_errorfor (-',i,')',pseudo_complement_targets_error [ -1 * i]
            i+=1
        pseudo_complement_targets_all.append(pseudo_complement_targets)

def generate_pseudo_overlapped_targets(pseudo_overlapped_targets_all,n):
    for i in range(0, len(pseudo_targets_all)):
        pseudo_overlapped_targets = {}
        for temp_bucket_number in range(0,n+1):
            pseudo_overlapped_targets[-1 *temp_bucket_number] = intersect_interval(pseudo_targets_all[i][-1 * temp_bucket_number],pseudo_complement_targets_all[i][-1 * temp_bucket_number])
        pseudo_overlapped_targets_all.append(pseudo_overlapped_targets)

def compute_interval_length(interval):
    interval_length = 0.0
    for i in range(0,len(interval)):
        interval_length += interval[i][1] - interval[i][0]
    return interval_length

def intersect_interval(interval1,interval2):
    interval3=[]
    len1=len(interval1)
    len2=len(interval2)
    i=0
    j=0
    while(i<len1 and j<len2):
        start1,end1=interval1[i]
        start2,end2=interval2[j]
        overlap = 0
        if(start1 >= start2 and start1 <= end2):
            overlap = 1
        if(start2 >= start1 and start2 <= end1):
            overlap = 1
        if(overlap == 1):
            start3 , end3 = max(start1,start2),min(end1,end2)
            if( start3 < end3):
                interval3.append([start3,end3])
        if(end1 <= end2):
            i+=1
        else:
            j+=1
    return interval3

# works now for multiple traces. Computes the entropy for target node. Checks the bias values and computes the result accordingly.
''' mean = sum of all the interval sizes for all traces which are intersection of end_match and target_interval / sum of all the interval
	sizes of target_interval
	entropy = -1 * mean_target * safe_log(mean_target,2)

	for bias = 1, pseudo_target_interval is considered
	for bias = -1, pseudo_complement_target_interval is considered
	for bias = 0, pseudo_target_interval, pseudo_complement_target_interval entropy are added and pseudo_overlapped_interval entropy is subtracted.
'''
def compute_entropy_for_current_node():
    if(bias == 1):
        num = 0
        den = 0
        for i in range(0, len(csv_filename)):
            temp_bucket_number = initial_minimum_bucket_value[i]
            end_match = intersected_influence_list_all[i][temp_bucket_number]
            if(end_match == []):
                end_match.append([0,init_matches[i]])
            
            pseudo_target_interval = pseudo_targets_all[i][-1 * temp_bucket_number]
            num+= compute_interval_length(intersect_interval(end_match, pseudo_target_interval))
            den+= compute_interval_length(end_match)
        mean_target = safe_div(num, den)
        entropy_interval = -1 * mean_target * safe_log(mean_target,2)
        return entropy_interval
        
    elif(bias == -1):
        num = 0
        den = 0
        for i in range(0, len(csv_filename)):
            temp_bucket_number = initial_minimum_bucket_value[i]
            end_match = intersected_influence_list_all[i][temp_bucket_number]
            if(end_match == []):
                end_match.append([0,init_matches[i]])
            
            pseudo_complement_target_interval = pseudo_complement_targets_all[i][-1 * temp_bucket_number]
            num+= compute_interval_length(intersect_interval(end_match, pseudo_complement_target_interval))
            den+= compute_interval_length(end_match)
        mean_complement_target = safe_div(num, den)
        entropy_complement_interval = -1 * mean_complement_target * safe_log(mean_complement_target,2)
        return entropy_complement_interval
    
    elif(bias != 0):
        print('Bias values cannot be anything other than 1,-1 and 0')
    
    else:
        print('well we have found the bias is 0 ')
        num_target = 0
        num_complemented_target = 0
        den_target = 0
        den_complemented_target = 0
        num_overlapped_target = 0
        den_overlapped_target = 0
        for i in range(0, len(csv_filename)):
            temp_bucket_number = initial_minimum_bucket_value[i]
            end_match = intersected_influence_list_all[i][temp_bucket_number]
            if(end_match == []):
                end_match.append([0,init_matches[i]])
            
            pseudo_target_interval = pseudo_targets_all[i][-1 * temp_bucket_number]
            pseudo_complement_target_interval = pseudo_complement_targets_all[i][-1 * temp_bucket_number]
            pseudo_overlapped_target_interval = pseudo_overlapped_targets_all[i][-1 * temp_bucket_number]
            num_target+= compute_interval_length(intersect_interval(end_match, pseudo_target_interval))
            den_target+= compute_interval_length(end_match)
            num_complemented_target+= compute_interval_length(intersect_interval(end_match, pseudo_complement_target_interval))
            den_complemented_target+= compute_interval_length(end_match)
            num_overlapped_target+= compute_interval_length(intersect_interval(end_match, pseudo_overlapped_target_interval))
            den_overlapped_target+= compute_interval_length(end_match)
        # print("*********")
        # print(num_target, den_target)
        # print(num_complemented_target, den_complemented_target)
        # print(num_overlapped_target, den_overlapped_target)
        # print("*********")    
        mean_target = safe_div(num_target, den_target)
        mean_complement_target = safe_div(num_complemented_target, den_complemented_target)
        mean_overlapped_target = safe_div(num_overlapped_target, den_overlapped_target)
    
        entropy_interval = -1 * mean_target * safe_log(mean_target,2)

        entropy_complement_interval = -1 * mean_complement_target * safe_log(mean_complement_target,2)
            
        entropy_overlap = mean_overlapped_target * safe_log(mean_overlapped_target,2)
        
        # print("*********")
        # print(entropy_interval)
        # print(entropy_complement_interval)
        # print(entropy_overlap)
        # print("*********")    
        
        entropy_final = entropy_interval + entropy_complement_interval + entropy_overlap
        return entropy_final

def find_column(sig_name):
    for index in range(0,len(signal_list)):
        if( signal_list[index][0] == sig_name):
            return signal_list[index][2],signal_list[index][1]

# run the booleanise tool to find the intervals in the csv where target_predicate is true. Returns list of list
def find_Interval(target_predicate, csv):
    global considered_predicate
    global considered_predicate_interval
    col_val, col_time, target = target_predicate
    considered_predicate = target_predicate
    f = open("test1",'w+')
    f.truncate()
    target_val = target.split()
    f.write("start 1\n\n")
    f.write(target_val[0]+" "+ str(col_val) + ' ' + str(col_time))
    f.write('\n\nbegin\n\n')
    f.write(target)
    f.write('\n\nend\n')
    f.write('(1)')
    f.close()
    booleanize_command = './Booleanize test1 ' + csv_filename[csv] + ' > uselessfile'
    os.system(booleanize_command)
    output_filename = csv_filename[csv].split('.')[0] + '.dat'
    f = open(output_filename,'r+')
    interval = []
    line = ''
    for row in f:
        line = row
    f.close()
    considered_predicate_interval = line
    if(line == '{}'):
        return interval
    firstline = True
    for i in line.split(')')[:-1]:
        if(firstline):
            line = i[2:]
            firstline = False
        else:
            line = i[1:]
        line1 = line.split(':')
        first, second = float(line1[0]), float(line1[1])
        if(second >= initial_trace_length):
            second = initial_trace_length
            stop_computing = 1
        if(first < initial_trace_length):
            interval.append([first,second])
    return interval

def compute_forward_influence_list(predicate_interval, bucket_number, csv):
    temp_minimum_bucket_value = min(bucket_number, initial_minimum_bucket_value[csv])
    i = bucket_number
    if(intersected_influence_list_all[csv][i] == []):
        temp_influence_entry_interval = predicate_interval
    else:
        temp_influence_entry_interval = intersect_interval(predicate_interval,intersected_influence_list_all[csv][i])
    i-=1
    while(i >= temp_minimum_bucket_value):
        temp_influence_entry_interval = minkowski_sum(temp_influence_entry_interval,k)
        if(intersected_influence_list_all[csv][i] == []):
            temp_influence_entry_interval = predicate_interval
        else:
            temp_influence_entry_interval = intersect_interval(predicate_interval,intersected_influence_list_all[csv][i])
        i -= 1
    return temp_influence_entry_interval

# works similar to compute_entropy_for_current_node(), computes the entropy after split and returns the error value.
# function computes entropy based on the bias value provided.

def find_error_for_predicate(predicate, bucket_number):
    print('finding error for ',predicate[2])
    
    if(bias == 1):
        num_predicate_true = 0
        den_predicate_true = 0
        num_predicate_false = 0
        den_predicate_false = 0
        predicate_true_length = 0
        predicate_false_length = 0
        predicate_total_length = 0
        for csv in range(0, len(csv_filename)):
            temp_minimum_bucket_value = min(bucket_number, initial_minimum_bucket_value[csv])
            predicate_interval = find_Interval(predicate, csv)
            complement_predicate_interval = complement_interval(predicate_interval)

            end_match_predicate_interval = compute_forward_influence_list(predicate_interval, bucket_number, csv)
            end_match_complement_predicate_interval = compute_forward_influence_list(complement_predicate_interval, bucket_number, csv)
            pseudo_target_interval = pseudo_targets_all[csv][-1 * temp_minimum_bucket_value]
            num_predicate_true+= compute_interval_length(intersect_interval(end_match_predicate_interval, pseudo_target_interval))
            den_predicate_true+= compute_interval_length(end_match_predicate_interval)
            
            num_predicate_false+= compute_interval_length(intersect_interval(end_match_complement_predicate_interval, pseudo_target_interval))
            den_predicate_false+= compute_interval_length(end_match_complement_predicate_interval)
            
            temp_true_length = compute_interval_length(end_match_predicate_interval)
            temp_false_length = compute_interval_length(end_match_complement_predicate_interval)
            predicate_true_length+= temp_true_length
            predicate_false_length+= temp_false_length
            predicate_total_length+= temp_true_length + temp_false_length
            
        mean_predicate_true = safe_div(num_predicate_true, den_predicate_true)
        mean_predicate_false = safe_div(num_predicate_false, den_predicate_false)
        entropy_predicate_true = -1 * mean_predicate_true * safe_log(mean_predicate_true, 2)
        entropy_predicate_false = -1 * mean_predicate_false * safe_log(mean_predicate_false, 2)
        probalility_predicate_true = safe_div(predicate_true_length, predicate_total_length)
        probability_predicate_false = safe_div(predicate_false_length, predicate_total_length)
        print('branch probabilities(weights) are')
        print(probalility_predicate_true, probability_predicate_false)
        error_with_split = (probalility_predicate_true * entropy_predicate_true) + (probability_predicate_false * entropy_predicate_false)
        return error_with_split
    
    elif(bias == -1):
        num_predicate_true = 0
        den_predicate_true = 0
        num_predicate_false = 0
        den_predicate_false = 0
        predicate_true_length = 0
        predicate_false_length = 0
        predicate_total_length = 0
        for csv in range(0, len(csv_filename)):
            temp_minimum_bucket_value = min(bucket_number, initial_minimum_bucket_value[csv])
            predicate_interval = find_Interval(predicate, csv)
            complement_predicate_interval = complement_interval(predicate_interval)

            end_match_predicate_interval = compute_forward_influence_list(predicate_interval, bucket_number, csv)
            end_match_complement_predicate_interval = compute_forward_influence_list(complement_predicate_interval, bucket_number, csv)
            pseudo_complement_target_interval = pseudo_complement_targets_all[csv][-1 * temp_minimum_bucket_value]
            num_predicate_true+= compute_interval_length(intersect_interval(end_match_predicate_interval, pseudo_complement_target_interval))
            den_predicate_true+= compute_interval_length(end_match_predicate_interval)
            
            num_predicate_false+= compute_interval_length(intersect_interval(end_match_complement_predicate_interval, pseudo_complement_target_interval))
            den_predicate_false+= compute_interval_length(end_match_complement_predicate_interval)
            
            temp_true_length = compute_interval_length(end_match_predicate_interval)
            temp_false_length = compute_interval_length(end_match_complement_predicate_interval)
            predicate_true_length+= temp_true_length
            predicate_false_length+= temp_false_length
            predicate_total_length+= temp_true_length + temp_false_length
            
        mean_predicate_true = safe_div(num_predicate_true, den_predicate_true)
        mean_predicate_false = safe_div(num_predicate_false, den_predicate_false)
        entropy_predicate_true = -1 * mean_predicate_true * safe_log(mean_predicate_true, 2)
        entropy_predicate_false = -1 * mean_predicate_false * safe_log(mean_predicate_false, 2)
        probalility_predicate_true = safe_div(predicate_true_length, predicate_total_length)
        probability_predicate_false = safe_div(predicate_false_length, predicate_total_length)
        print('branch probabilities(weights) are')
        print(probalility_predicate_true, probability_predicate_false)
        error_with_split = (probalility_predicate_true * entropy_predicate_true) + (probability_predicate_false * entropy_predicate_false)
        return error_with_split
    
    elif(bias != 0):
        print('Bias values cannot be anything other than 1,-1 and 0')
    
    else:
        print('well we have found the bias is 0 ')
        num_predicate_true_target, den_predicate_true_target = 0, 0
        num_predicate_true_complemented_target, den_predicate_true_complemented_target = 0, 0
        num_predicate_true_overlapped_target, den_predicate_true_overlapped_target = 0, 0
        
        num_predicate_false_target, den_predicate_false_target = 0, 0
        num_predicate_false_complemented_target, den_predicate_false_complemented_target = 0, 0
        num_predicate_false_overlapped_target, den_predicate_false_overlapped_target = 0, 0
        
        predicate_true_length = 0
        predicate_false_length = 0
        predicate_total_length = 0
        
        for csv in range(0, len(csv_filename)):
            temp_minimum_bucket_value = min(bucket_number, initial_minimum_bucket_value[csv])
            predicate_interval = find_Interval(predicate, csv)
            complement_predicate_interval = complement_interval(predicate_interval)

            end_match_predicate_interval = compute_forward_influence_list(predicate_interval, bucket_number, csv)
            end_match_complement_predicate_interval = compute_forward_influence_list(complement_predicate_interval, bucket_number, csv)
            
            pseudo_target_interval = pseudo_targets_all[csv][-1 * temp_minimum_bucket_value]
            pseudo_complement_target_interval = pseudo_complement_targets_all[csv][-1 * temp_minimum_bucket_value]
            pseudo_overlapped_target_interval = pseudo_overlapped_targets_all[csv][-1 * temp_minimum_bucket_value]
            
            num_predicate_true_target+= compute_interval_length(intersect_interval(end_match_predicate_interval, pseudo_target_interval))
            den_predicate_true_target+= compute_interval_length(end_match_predicate_interval)
            num_predicate_true_complemented_target+= compute_interval_length(intersect_interval(end_match_predicate_interval, pseudo_complement_target_interval))
            den_predicate_true_complemented_target+= compute_interval_length(end_match_predicate_interval)
            num_predicate_true_overlapped_target+= compute_interval_length(intersect_interval(end_match_predicate_interval, pseudo_overlapped_target_interval))
            den_predicate_true_overlapped_target+= compute_interval_length(end_match_predicate_interval)
            
            num_predicate_false_target+= compute_interval_length(intersect_interval(end_match_complement_predicate_interval, pseudo_target_interval))
            den_predicate_false_target+= compute_interval_length(end_match_complement_predicate_interval)
            num_predicate_false_complemented_target+= compute_interval_length(intersect_interval(end_match_complement_predicate_interval, pseudo_complement_target_interval))
            den_predicate_false_complemented_target+= compute_interval_length(end_match_complement_predicate_interval)
            num_predicate_false_overlapped_target+= compute_interval_length(intersect_interval(end_match_complement_predicate_interval, pseudo_overlapped_target_interval))
            den_predicate_false_overlapped_target+= compute_interval_length(end_match_complement_predicate_interval)
            
            temp_true_length = compute_interval_length(end_match_predicate_interval)
            temp_false_length = compute_interval_length(end_match_complement_predicate_interval)
            predicate_true_length+= temp_true_length
            predicate_false_length+= temp_false_length
            predicate_total_length+= temp_true_length + temp_false_length
        
        mean_predicate_true_target = safe_div(num_predicate_true_target, den_predicate_true_target)
        mean_predicate_false_target = safe_div(num_predicate_false_target, den_predicate_false_target)
        mean_predicate_true_complemented_target = safe_div(num_predicate_true_complemented_target, den_predicate_true_complemented_target)
        mean_predicate_false_complemented_target = safe_div(num_predicate_false_complemented_target, den_predicate_false_complemented_target)
        mean_predicate_true_overlapped_target = safe_div(num_predicate_true_overlapped_target, den_predicate_true_overlapped_target)
        mean_predicate_false_overlapped_target = safe_div(num_predicate_false_overlapped_target, den_predicate_false_overlapped_target)
        
        entropy_predicate_true_target = -1 * mean_predicate_true_target * safe_log(mean_predicate_true_target, 2)
        entropy_predicate_false_target = -1 * mean_predicate_false_target * safe_log(mean_predicate_false_target, 2)
        entropy_predicate_true_complemented_target = -1 * mean_predicate_true_complemented_target * safe_log(mean_predicate_true_complemented_target, 2)
        entropy_predicate_false_complemented_target = -1 * mean_predicate_false_complemented_target * safe_log(mean_predicate_false_complemented_target, 2)
        entropy_predicate_true_overlappped_target = mean_predicate_true_overlapped_target * safe_log(mean_predicate_true_overlapped_target, 2)
        entropy_predicate_false_overlapped_target = mean_predicate_false_overlapped_target * safe_log(mean_predicate_false_overlapped_target, 2)
        
        entropy_predicate_true = entropy_predicate_true_target + entropy_predicate_true_complemented_target + entropy_predicate_true_overlappped_target
        entropy_predicate_false = entropy_predicate_false_target + entropy_predicate_false_complemented_target + entropy_predicate_false_overlapped_target
        
        probalility_predicate_true = safe_div(predicate_true_length, predicate_total_length)
        probability_predicate_false = safe_div(predicate_false_length, predicate_total_length)
        print('branch probabilities(weights) are')
        print(probalility_predicate_true, probability_predicate_false)
        error_with_split = (probalility_predicate_true * entropy_predicate_true) + (probability_predicate_false * entropy_predicate_false)
        return error_with_split

# stores top m predicates with the best gain value
def store_m_best_predicates(gain, predicate, bucket_number):
    if gain <= 0:
        return

    for i in range(len(m_best_predicates)):
        item_gain, item_predicate, item_bucket_number = m_best_predicates[i]
        if(item_gain == gain and item_bucket_number == bucket_number):
            item_predicate_signalname, item_predicate_operator, item_predicate_constant = item_predicate
            predicate_signalname, predicate_operator, predicate_constant = predicate

            if(item_predicate_signalname == predicate_signalname and item_predicate_operator == predicate_operator):
                if(predicate_operator == '>=' and predicate_constant >= item_predicate_constant):
                    m_best_predicates[i][1] = predicate
                    return
                if(predicate_operator == '<=' and predicate_constant <= item_predicate_constant):
                    m_best_predicates[i][1] = predicate
                    return
                return

    if((gain, predicate, bucket_number) in m_best_predicates):
        print('predicate already added to priority list')
        return

    index_new_predicate = 0
    len_m_best_predicates = len(m_best_predicates)
    if(len_m_best_predicates == 0):
        m_best_predicates.append((gain, predicate, bucket_number))
        return
    else:
        while (index_new_predicate < len_m_best_predicates):
            if(gain > m_best_predicates[index_new_predicate][0]):
                break
            index_new_predicate+= 1
    m_best_predicates.insert(index_new_predicate, (gain, predicate, bucket_number))
    print('predicate =', predicate)
    if(len(m_best_predicates) > m):
        del m_best_predicates[-1]

def find_min_max_of_variable(i):
    min_list = list()
    max_list = list()
    for x in minmax_of_signal_variables:
        for y in x:
            if(y[0] == i):
                min_list.append(y[1])
                max_list.append(y[2])
   
    return min(min_list), max(max_list)

# helper function for simulated annealing. sa() 
# temperature variation according to linear schedule
def generate_predicate(i, op, bucket_number, curr_error):
    col_val, col_time = find_column(i)
    min_val, max_val = find_min_max_of_variable(i)
    
    store_error = {}

    print('\n\n signal',i,'max_val= ',max_val,'min_val = ',min_val,'bucket number =',bucket_number)
    if(min_val == max_val):
        return 0,"blank",[]

    #temperature already set in main function
    #setting temperature parameters
    T = Tmax
    const_val = (max_val + min_val)/2
    constraint = i + ' ' + op + ' ' + str(const_val)

    init_pred = (col_val, col_time, constraint)
    error = find_error_for_predicate(init_pred, bucket_number)
    store_error[const_val] = error
    # gain is how much the error is reduced

    print('error target = ',curr_error)
    print('error after split',error)

    gain = curr_error - error
    print('predicate ',init_pred,'gain ',gain)

    store_m_best_predicates(gain, init_pred, bucket_number)

    while(T > Tmin):
        displacement = (T - Tmin) * 1.0/(Tmax - Tmin)
        const_val_left = const_val - ((const_val - min_val) * displacement)
        const_val_right = const_val + ((max_val - const_val) * displacement)
        print('\n const val = ', const_val,'displacement = ', displacement, 'const_val_left ', const_val_left, 'const_val_right ', const_val_right)

        #finding error for left val
        if const_val_left in store_error:
            error_left = store_error[const_val_left]
            gain_left = curr_error - error_left
        else:
            constraint_left = i + ' ' + op + ' ' +str(const_val_left)
            init_pred_left = (col_val,col_time,constraint_left)
            error_left = find_error_for_predicate(init_pred_left, bucket_number)
            store_error[const_val_left] = error_left
            gain_left = curr_error - error_left
            store_m_best_predicates(gain_left,init_pred_left,bucket_number)

        #finding error for right value
        if const_val_right in store_error:
            error_right = store_error[const_val_right]
            gain_right = curr_error - error_right
        else:
            constraint_right =i + ' ' + op + ' ' + str(const_val_right)
            init_pred_right = (col_val, col_time, constraint_right)
            
            error_right = find_error_for_predicate(init_pred_right, bucket_number)
            store_error[const_val_right] = error_right
            gain_right = curr_error - error_right
            store_m_best_predicates(gain_right, init_pred_right, bucket_number)

        print('gain = ',gain,'gain_left ',gain_left,'gain_right = ',gain_right)

        if(gain_left > gain and gain_left >= gain_right):
            const_val = const_val_left
            gain = gain_left
        elif(gain_right > gain and gain_right >= gain_left):
            const_val = const_val_right
            gain = gain_right
        elif(gain > gain_right and gain > gain_left):
            #if new solution is not good accept it with some probability
            if(gain_left >= gain_right):
                if( displacement > random.random()):
                    const_val = const_val_left
                    gain = gain_left
                    
            else:
                if( displacement > random.random()):
                    const_val = const_val_right
                    gain = gain_right

        #decrease temperature
        T-= 1

    constraint = i + ' ' + op + ' ' + str(const_val)
    init_pred = (col_val, col_time, constraint)

    return gain, init_pred

# get interval string from list of list, returned by find_Interval() function.
def get_interval_string(interval):
	if (len(interval) == 0):
		res = '{}'
		return res
	separator = ''
	sep_list = str(interval).split('], ')
	sep_list[0] = sep_list[0].replace('[', '', 1)
	sep_list[-1] = sep_list[-1].replace(']', '')
	print(sep_list)
	for i in range(0, len(sep_list)):
		sep_list[i] = sep_list[i] + ')'
	res_interval = separator.join(sep_list)
	res_interval = res_interval.replace(', ', ' : ')
	res_interval = '{' + res_interval + '}'
	return res_interval

def print_m_best_predicates():
    fileptr = open('best_predicates.txt', 'w')
    fileptr.truncate()

    print('index \t gain \t\t predicate \t\t bucket_number \n')
    for i in range(0, len(m_best_predicates)):
        fileptr.write("begin_pred\n")
        fileptr.write(str(m_best_predicates[i][1][2]) + '\n' )
        fileptr.write('gain = ' + str(m_best_predicates[i][0]) + '\n')
        fileptr.write('bucket = ' +  str(m_best_predicates[i][2]) + '\n')
        fileptr.write('intervals_begin\n')
        for csv in range(0, len(csv_filename)):
            fileptr.write(str(csv) + ' ')
            inter = find_Interval(m_best_predicates[i][1], csv)
            inter_string = get_interval_string(inter)
            fileptr.write(str(inter_string) + ' \n')
        fileptr.write('intervals_end\n')

        fileptr.write('end_pred\n\n')
        print(i + 1,'\t',m_best_predicates[i][0],'\t',m_best_predicates[i][1][2],'\t',m_best_predicates[i][2])
    fileptr.close()

def cannot_learn_need_to_exit():
    print('Since the error is 0 we cannot learn anything here')
    fileptr = open('best_predicates.txt','w')
    fileptr.truncate()
    fileptr.close()
    sys.exit()

def sa():
    #local variables 
    max_gain = 0.0
    best_predicate = (-1,-1,'val >= 0')
    any_variable_to_add = 0
    best_bucket_number = -1
    curr_error = compute_entropy_for_current_node()
    
    print('current Error for node is ',curr_error)
    if (curr_error == 0):
        cannot_learn_need_to_exit()
    print(csv_filename, operator_list)
    #for every signal variable check best predicate that could be generated.
    #print(processed_signal_variable)
    for signal_variable_name in processed_signal_variable:
        if(processed_signal_variable[signal_variable_name] == 0):
            #here we are calling function that returns us best predicate for i th signal variable for every operator in operator list
            gain = -1
            for op in operator_list:
                for bucket_number in range(n + 1):
                    gain_op, predicate_op = generate_predicate(signal_variable_name,op, bucket_number , curr_error)
                if(gain_op > gain):
                    gain,predicate,pred_bucket_number = gain_op,predicate_op,bucket_number

            any_variable_to_add = 1

            if(gain <= 0):
                print('got gain 0 for signal variable ',signal_variable_name)
                processed_signal_variable[signal_variable_name] = -1
            else:
                print('For signal variable ',signal_variable_name,'simulated annealing has selected predicate',predicate)
                print('at position ',pred_bucket_number,'getting a gain of',gain)
                if(gain > max_gain):
                    max_gain = gain
                    best_predicate = predicate
                    best_bucket_number = pred_bucket_number

def get_traces():
    for i in csv_filename:
        df = pd.read_csv(i)
        col = df[df.columns[signal_list[0][1] - 1]]
        val = max(col) - min(col)
        init_matches.append(val)

def delete_files():
	for csv_file in csv_filename:
		command1 = 'rm minimum_maximum_' + csv_file[:-4] +'.txt'
		os.system(command1)
		command2 = 'rm ' + csv_file[:-4] + '.dat'
		os.system(command2)
	os.system('rm test1')
	os.system('rm translate.py')
	os.system('rm uselessfile')

if __name__ == "__main__":
    arguments = sys.argv
    # list of signal variables [signal_variable_name , column_time , column_value]
    global signal_list
    signal_list = []
    # storing minimum and maximum of signal varibale in form signal_variable_name , minimum value , 
    # maximum value
    global minmax_of_signal_variables
    minmax_of_signal_variables = []
    # list of buckets of the intervals [bucket number,list of interval]
    global influence_list_all
    influence_list_all = {}
    global considered_predicate
    considered_predicate = (-1,-1,'val >= -1')
    global considered_predicate_interval
    considered_predicate_interval = 'nothing as of now'

    # list of buckets of the intervals [bucket number,list of interval] where every interval 
    # is intersected with previous
    # with resolution [0:k]
    global intersected_influence_list_all
    intersected_influence_list_all = []

    # arguments to read from config csv_filename,m,k,Tmax,Tmin,trace_length,target_interval
    other_arguments = []

    global target_interval
    target_interval = []

    global target_false_interval
    target_false_interval = []

    global pseudo_targets_all
    pseudo_targets_all = []

    global pseudo_complement_targets_all
    pseudo_complement_targets_all = []

    global pseudo_overlapped_targets_all
    pseudo_overlapped_targets_all = []

    # global pseudo_complement_targets_error
    # pseudo_complement_targets_error = {}

    global m_best_predicates
    m_best_predicates = []

    global m_best_predicates_interval
    m_best_predicates_interval = []

    # list of operators to take into consideration while generating predicates
    global operator_list
    operator_list = ['>=', '<=']

    blockPrint()

    global init_matches
    init_matches = []

    if(len(arguments) < 2):
        # print('config filename not given in argument so by default taking config_file_constrained.txt')
        config_file_name = 'config_file_constrained.txt'
    else:
        config_file_name = arguments[1]
        # print('calling read_config')

    #config_file_name = 'surv.cfg'

    read_from_config(config_file_name, signal_list, other_arguments)

    global csv_filename
    csv_filename = other_arguments[0]

    global m 
    m = int(other_arguments[1])
    k = float(other_arguments[2])
    global Tmax
    global Tmin
    Tmax = int(other_arguments[3])
    Tmin = int(other_arguments[4])
    global n
    n = int(other_arguments[5])
    global trace_length
    trace_length = float(other_arguments[6])
    global initial_trace_length
    initial_trace_length = trace_length
    target_interval = other_arguments[7]
    
    print(signal_list)

    global bias
    bias = int(other_arguments[8])

    global strict
    strict = int(other_arguments[9])

    target_false_interval = [complement_interval(i) for i in target_interval]

    get_traces()
    print(init_matches)

    global initial_minimum_bucket_value
    initial_minimum_bucket_value = []

    for influence_list in influence_list_all.values():
        sort_influence_list(influence_list)
        initial_minimum_bucket_value.append(get_min_bucket_value(influence_list))
        fill_intersected_influence_list(n, influence_list)

    generate_pseudo_targets(pseudo_targets_all,n,k)

    generate_pseudo_complement_targets(pseudo_complement_targets_all,n,k)

    generate_pseudo_overlapped_targets(pseudo_overlapped_targets_all,n)
    
    global processed_signal_variable
    processed_signal_variable = {}
    for i in signal_list:
        processed_signal_variable[i[0]] = 0

    # # the function computes minimum and maximum value of every signal variable and stores it in list minmax of signla variable
    for i in csv_filename:
        compute_min_max_of_signal_variable(i, signal_list, minmax_of_signal_variables)

    # #the signal variable whose min and max values are same will not contribute and hence are eliminated
    for i in minmax_of_signal_variables:
        eliminate_useless_signals(processed_signal_variable , i)

    print(minmax_of_signal_variables)
    sa()
    print(m_best_predicates)
    print_m_best_predicates()

    delete_files()
