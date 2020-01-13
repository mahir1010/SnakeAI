# SnakeAI
## An AI agent for the [SnakeClassic](https://mahir1010.github.io/SnakeClassic/).
### Define following variables in ``compile.sh``
   * SnakeAIPath: Path to the root.
   * CUDA_PATH: Path to the root of CUDA installation
   * search_depth: Creates kernel with width of (search_depth*2+1)
        
Example: &nbsp; search_depth=3

| -3 | -3 | -3 | -3 | -3 | -3 | -3 |
|----|----|----|----|----|----|----|
| -3 | -2 | -2 | -2 | -2 | -2 | -3 |
| -3 | -2 | -1 | -1 | -1 | -2 | -3 |
| -3 | -2 | -1 | 0  | -1 | -2 | -3 |
| -3 | -2 | -1 | -1 | -1 | -2 | -3 |
| -3 | -2 | -2 | -2 | -2 | -2 | -3 |
| -3 | -3 | -3 | -3 | -3 | -3 | -3 |

**Methods**
1. Greedy Locally Optimum - Score 249 
![greedy locally optimum](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/locally%20optimum.gif)

2. Probabilistic Heuristic - Score 1001  
![prob](https://raw.githubusercontent.com/mahir1010/SnakeAI/screenshot/Probabilistic%20Heuristic.gif)
