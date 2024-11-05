RED='\033[0;31m'
NC='\033[0m'

g++ -pthread main.cpp src/tools.cpp -I include -o main


./main -t 16 -r 200000000 &
PROG_PID=$!

echo -e "${RED}Program PID: $PROG_PID${NC}"
echo -e "${RED}Monitoring threads...${NC}"


while kill -0 $PROG_PID 2>/dev/null; do
    echo -e "${RED}$(date +%T%3N) - Number of threads: $(ls /proc/$PROG_PID/task | wc -l)${NC}"
    sleep 0.05
done

wait $PROG_PID