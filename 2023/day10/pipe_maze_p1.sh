#!/bin/bash

# https://adventofcode.com/2023/day/10

## part 1

# Enable extended globbing to use [[ $my_variable == @(a|b|c) ]]
shopt -s extglob

SCRIPT_DIR=$PWD/2023/day10/
INPUT_DATA=$SCRIPT_DIR/input_data

##unicode symbols: https://www.compart.com/en/unicode/html

declare -A GRID

function load_data() {
    local index=0
    while read -r line; do
        echo "$line" | sed 's/F/┌/g;s/7/┐/g;s/L/└/g;s/J/┘/g;s/-/─/g;s/|/│/g'
        GRID[$index]="$line"
        ((index++))
    done #< <(cat | sed 's/F/┌/g;s/7/┐/g;s/L/└/g;s/J/┘/g;s/-/─/g;s/|/│/g')
    ##symbols unicode: U+250C, U+2510, U+2514, U+2518, U+2500, U+2502
}

function find_start() {
    local start_char="S"

    index="$(awk -v char="$start_char" \
        'BEGIN{OFS="\t"} {pos=index($0, char); if (pos > 0) print NR, pos;}' \
        "$INPUT_DATA" | tr "\t" " " )"
    
    index="$(( ${index%% *} -1 )),$(( ${index#* } -1 ))"
    echo "$index"
}

function navigate()
{
    row_lenght=$(echo -n "${GRID[0]}" | wc -c)
    col_lenght=${#GRID[@]}
    start_row=${START_POSITION%%,*}
    start_col=${START_POSITION#*,}
    echo "ln_lght:$row_lenght ln_num:$col_lenght start_row:$start_row start_col:$start_col"
    prev_col=null
    prev_row=null
    row=""
    col=""
    steps=0
    while true; do
            ((steps++))
            [[ -z $row ]] && row=$start_row
            [[ -z $col ]] && col=$start_col
            echo "pos:($row,$col) ${GRID[$row]:$((col)):1}"
            if [[ $((col-1)) -gt -1 && "$row,$((col-1))" != "$prev_row,$prev_col" ]]; then ##look left
                #echo "left:$row,$((col-1)) ${GRID[$row]:$((col-1)):1}"
                if [[ ${GRID[$row]:$((col)):1} == @(S|-|J|7) ]]; then
                    case ${GRID[$row]:$((col-1)):1} in
                    "-" | "L" | "F")
                        ##move left
                        prev_row=$row
                        prev_col=$col
                        col=$((col-1))
                        continue
                    ;;
                    esac
                fi
            fi
            if [[ $((col+1)) -le $col_lenght+1 && "$row,$((col+1))" != "$prev_row,$prev_col" ]]; then ##look right
                #echo "right:$row,$((col+1)) ${GRID[$row]:$((col+1)):1}"
                if [[ ${GRID[$row]:$((col)):1} == @(S|-|L|F) ]]; then
                    case ${GRID[$row]:$((col+1)):1} in
                    "-" | "J" | "7")
                        ##move right
                        prev_row=$row
                        prev_col=$col
                        col=$((col+1))
                        continue
                    ;;
                    esac
                fi
            fi
            if [[ $((row-1)) -gt -1 && "$((row-1)),$col" != "$prev_row,$prev_col" ]]; then ##look up
                #echo "up:$((row-1)),$col ${GRID[$((row-1))]:$col:1}"
                if [[ ${GRID[$row]:$((col)):1} == @(S|L|J|\|) ]]; then
                    case ${GRID[$((row-1))]:$col:1} in
                    "|" | "7" | "F" )
                        ##move up
                        prev_row=$row
                        prev_col=$col
                        row=$((row-1))
                        continue
                    ;;
                    esac
                fi
            fi
            if [[ $((row+1)) -le $row_lenght+1 && "$((row+1)),$col" != "$prev_row,$prev_col" ]]; then ##look down
                #echo "down:$((row+1)),$col ${GRID[$((row+1))]:$col:1}"
                if [[ ${GRID[$row]:$((col)):1} == @(S|\||7|F) ]]; then
                    case ${GRID[$((row+1))]:$col:1} in
                    "|" | "J" | "L" )
                        ##move down
                        prev_row=$row
                        prev_col=$col
                        row=$((row+1))
                        continue
                    ;;
                    esac
                fi
            fi
            echo "Back at start. Can not go further."
            echo "Steps to farthest postiion:$((steps/2))"
            break
    done
}

load_data < "$INPUT_DATA"

echo ""
for ((i=0;i<${#GRID[@]};i++)); do
  printf "%s\n" "${GRID[$i]}"
done

START_POSITION=$(find_start)

#echo "$START_POSITION"

navigate