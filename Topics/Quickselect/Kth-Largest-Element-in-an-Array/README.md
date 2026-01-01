# Kth Largest Element in an Array
Problem link: [215. Kth Largest Element in an Array](https://leetcode.com/problems/kth-largest-element-in-an-array/description/?envType=problem-list-v2&envId=dsa-sorting-plateau-counting-sort-merge-sort-quickselect)

### Overview
This problem is an excellent use case for **Quickselect**. Conceptually, Quickselect resembles binary search, but with a crucial difference: binary search requires a fully sorted array, whereas Quickselect operates on an unsorted array and only partially orders elements as needed.
Quickselect works by repeatedly partitioning the array around a chosen pivot and then recursing into only one side of the partition. This recursing is similar in idea to binary search, and the partitioning is what differs and allows Quickselect to work on a non-sorted array.

The most common form of Quickselect uses **two-way partitioning**, where the array is divided into two zones (also called regions, but referred to here as zones). One zone contains elements less than the pivot, and the other contains elements greater than the pivot. Elements equal to the pivot are not handled specially and may end up in either zone, depending on the partitioning scheme used. Depending on the goal and state, we either return the pivot (if it is determined to be the desired element) or recurse into the left or right partition. Two most common two-way partitioning schemes are Lomuto’s and Hoare’s.

### Three-way Partitioning
**Three-way partitioning** is another option for Quickselect. Instead of partitioning into groups of <= and >= for some pivot, three-way partitioning partitions into zones of <, >, and = to the pivot. Three-way partitioning is especially beneficial if the array may contain many duplicate values so that we do not end up using the same value as a pivot repeatedly. The problem statement tells us that there may be many duplicates in the array, telling us that a three-way partitioning may be more beneficial than a two-way one for this problem. A common scheme for the three-way partition is the Dutch flag variation, and is the method that will be used for this solution. As well, this solution will implement an iterative, not recursive, approach.

### Note
It tends to be more intuitive and common for Quickselect implementations to find the kth smallest element. However, this problem asks for the kth largest element. To adapt Quickselect to find the kth largest element, we can trivially convert the problem into finding the (n - k)th smallest element, where n is the size of the array. This conversion allows us to use the same partitioning logic while adjusting our target index accordingly.

Call g = n - k. Thus, finding the kth largest element in one-based indexing (as described by the problem) is equivalent to finding the **gth smallest element in 
zero-based indexing**. Thus, throughout this explanation, we will refer to our target index as g = n - k.

### Understanding the implementation of Quickselect using a Three-way partition
**Let's take a look through the process of implementing Quickselect using a three-way partitioning**!

To begin the algorithm, we define our initial search range. Since we are looking for the gth smallest element (or equivalently the kth largest element) in the **entire** dataset, we initialize: **l** (left-most index of the array being searched) to 0 (the start of the array), and r (right-most index) to n - 1 (the end of the array). As the algorithm progresses, we will update these values as necessary to "zoom in" on the specific section of the array that must contain our target (similarly to with binary search).

With this initial search range established, we repeatedly refine it until we determine that the target element is found. While the target has not yet been identified, we repeat the following three-step process:
1. Choosing a pivot
2. Performing a three-way partition of the current range and the chosen pivot 
3. Decide which side to recurse/"zoom" into (or decide that we have found our target)

Let's work through these steps in more detail, and see how they function together to successfully find the gth smallest element (or equivalently the kth largest element). 

As listed above, the first step of our repeated process is to choose a pivot value. For this solution, we will choose the middle element. Although any element can be chosen as the pivot without affecting correctness, the pivot choice influences performance; choosing the middle element provides a simple, deterministic heuristic. 

The range of the array being searched at each iteration is defined by "l" (leftmost-index) and "r" (right-most index). Thus, let us call the pivot value **"pivot"**, which is equal to the value at the middle index of the array, calculated as `pivot = arr[l + (r - l) / 2]`.  

The second step of our repeated process is to perform a three-way partition of the current range `[l ... r]` using the chosen pivot value. 

In a three-way partition, we maintain three pointers with the goal being to partition the array into three distinct zones through rearrangement. These three zones are defined as containing elements as follows:
- Less than pivot: Elements smaller than our pivot value.
- Equal to pivot: All instances of the pivot value.
- Greater than pivot: Elements larger than our pivot value.

Let us call these three *pointers* **"lt"** (less than), **"i"** (current), and **"gt"** (greater than). The meaning of these pointers are as follows:

|        |                                                                                                                                                   |
|--------|---------------------------------------------------------------------------------------------------------------------------------------------------|
| **lt** | Starts at the beginning; moves right when we find a value smaller than the pivot. Initialized to l (the beginning of the array being partitioned) |
| **i**  | Starts at the beginning; the active scanner that moves until it hits gt. Initialized to l (the beginning of the array being partitioned)          |
| **gt** | Starts at the end; moves left when we find a value larger than the pivot. Initialized to r (the end of the array being partitioned)               |

Through the five pointers we have discussed so far — **l** (leftmost index of array being partitioned), **r** (rightmost index), **lt**,  **i**, and **gt** — we find that the array is constantly being rearranged into four zones:
1. The "Less Than" Zone [l, lt-1]: Elements strictly smaller than the pivot. 
2. The "Equal To" Zone [lt, i-1]: Elements exactly equal to the pivot. 
3. The "Unexplored" Zone [i, gt]: Elements we haven't checked yet. 
4. The "Greater Than" Zone [gt+1, r]: Elements strictly larger than the pivot.

As the i pointer moves across the array, it acts like a rearranger, "throwing" small values to the left (lt) and large values to the right (gt).

### The Mechanics of the Rearrangement

As we iterate through the **Unexplored Zone**, the pointer **i** (our scanner) encounters one of three scenarios. Each scenario dictates how we rearrange the elements to maintain our zones. Note that to maintain our zones, we are essentially maintaining the invariant that `[l..lt-1] < pivot` ("Less Than" Zone), `[lt..i-1] = pivot` ("Equal To" Zone), and `[gt+1..r] > pivot` ("Greater Than" Zone). 

* **If `arr[i] < pivot`**:
  We have found a "small" element. It belongs in the **Less Than** zone. We swap `arr[i]` with the element at `lt`. Because we know the element swapped to `i` is now an "Equal" element (pushed from the old `lt` boundary), we can safely move both pointers forward (`lt++`, `i++`).
* **If `arr[i] > pivot`**:
  We have found a "large" element. It belongs in the **Greater Than** zone. We swap `arr[i]` with the element at `gt` and decrement `gt--`. **Crucially**, we do not move `i` forward yet. We must re-examine the new element at `i` that was just swapped from the back, as it is still "Unexplored."
* **If `arr[i] == pivot`**:
  The element is already where it belongs (in the **Equal** zone). We simply increment `i++` to expand the equal zone.


### Terminating the Rearrangement Loop and Narrowing the Search

The partitioning/rearrangement loop ends when the scanner **i** passes the **gt** pointer (so it iterates `while(i <= gt)`). At this point, the "Unexplored Zone" has vanished, and the array is organized like so:

| Zone Range | Content |
| --- | --- |
| `[l ... lt-1]` | All elements **strictly less** than pivot. |
| `[lt ... gt]` | All elements **exactly equal** to pivot. |
| `[gt+1 ... r]` | All elements **strictly greater** than pivot. |

Now comes our third and last step of the repeated process: deciding which side to "zoom" into next, or whether we have found our target.

If we find that the `g` index lies within the **Equal To** zone `[lt ... gt]`, then we have found our gth smallest (or kth largest) element! After three-way partitioning, all elements in indices [lt … gt] are equal to the pivot, all elements to the left are strictly smaller, and all elements to the right are strictly larger; therefore, if the target index g lies within [lt … gt], one of the elements in the array with value equal to pivot has exactly the correct rank and must be the gth smallest (equivalently, kth largest) element. Thus, we know that the pivot value is the value of the gth smallest (equivalently, kth largest) element, and we thus return it immediately.

Otherwise, we will perform the "Select" part of Quickselect and "zoom" into either the "Less than" or "Greater than" zones. Since we are looking for the gth smallest element, we check where the index g falls relative to these boundaries. 

If `g < lt`, the  element must be in the "Less Than" zone. We update our right boundary `r = lt - 1` and repeat the process.  Otherwise, if `g > gt` the element must be in the "Greater Than" zone. We update our left boundary `l = gt + 1`. 

One last thing to note is what happens when the search range becomes very small. At the end of the process above, it is possible that we reach l == r, meaning there is exactly one element left in the current search range. In that case, this remaining element must be the answer, so we can safely return arr[l] (equivalently arr[r], since l == r).

As well, with a valid g/k and a correct implementation, the range should never become invalid. In other words, we should not end up with r < l before returning an answer. If r < l occurs, it indicates an implementation error (for example, comparing the wrong target index or updating bounds incorrectly), rather than a “not found” case.

If the above is not the case and l < r, we would then repeat our three-step process on our now narrowed zone  `[l ... r]`.

Through all this, we have outlined the mechanics of implementing a solution to this problem through Quickselect using a three-way partitioning scheme. By continually narrowing our search range based on the pivot and the zones created, we efficiently hone in on the desired kth largest element (or gth smallest element).

### Why this is efficient

By using the **Equal To** zone `[lt...gt]`, we handle duplicates in a single pass. If the array is full of the same number, the first partition will result in `lt = l` and `gt = r`. The algorithm will see that g is within that range and return the answer in O(n) time, whereas a standard two-way partition might struggle to split the array effectively, potentially slowing down to O(n^2).

### Implementation
Using this three-way partitioning Quickselect approach, we can implement the solution in C++. See Solution.cpp under this folder for this implementation 

**Complexity Analysis**
- **Time Complexity:**
  - Average: `O(n)`, because we discard a large portion of the array at each step. 
  - Worst Case: `O(n^2)`, though the three-way partition and middle-pivot strategy makes this unlikely for many practical inputs. Using a randomized-pivot selection instead of simply selecting the middle element as the pivot can help further avoid worst-case scenarios. 
- **Space Complexity:**
  - `O(1)`, as we are using an iterative approach and sorting the array **in-place**.