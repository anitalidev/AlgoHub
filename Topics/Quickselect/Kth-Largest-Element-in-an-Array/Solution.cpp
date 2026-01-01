class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int l = 0;
        int r = nums.size() - 1;

        // Convert "kth largest" to the target index in a sorted array.
        // e.g., in an array of size 10, the 1st largest is at index 9.
        int targetIndex = nums.size() - k;

        while (l <= r) {
            // Choose the middle element as the pivot
            int pivot = nums[l + (r - l) / 2];

            // Three-way partition pointers:
            // lt: boundary for elements < pivot
            // i:  current element being scanned (explorer)
            // gt: boundary for elements > pivot
            int lt = l;
            int i = l;
            int gt = r;

            while (i <= gt) {
                if (nums[i] < pivot) {
                    // Element is smaller; send to the left zone
                    swap(nums[i], nums[lt]);
                    i++;
                    lt++;
                } else if (nums[i] > pivot) {
                    // Element is larger; send to the right zone
                    // Note: We don't increment i here because the swapped
                    // element from gt needs to be inspected.
                    swap(nums[i], nums[gt]);
                    gt--;
                } else {
                    // Element is equal to pivot; just expand the equal zone
                    i++;
                }
            }

            // After partitioning:
            // [l ... lt-1] contains elements < pivot
            // [lt ... gt]  contains elements == pivot
            // [gt+1 ... r] contains elements > pivot

            // Check if our target index falls within the "Equal" zone
            if (targetIndex >= lt && targetIndex <= gt) {
                return pivot;
            }
                // If target is to the left of the equal zone, narrow search to the left
            else if (targetIndex < lt) {
                r = lt - 1;
            }
                // If target is to the right, narrow search to the right
            else {
                l = gt + 1;
            }
        }
        return -1;
    }
};