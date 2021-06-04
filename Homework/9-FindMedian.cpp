/* description: https://202.120.40.8:30463/contest/11/problem/1
 * start: 2021-6-3
 * timelimit: 7days
 */

#include <iostream>
#include <vector>

int find_k_element(std::vector<int>&nums1, std::vector<int>& nums2, int k) {
    int len1 = nums1.size();
    int len2 = nums2.size();

    int pos1 = 0, pos2 = 0;

    while (true) {
        // end the loop
        if (pos1 == len1)
            return nums2[pos2 + k - 1];
        if (pos2 == len2)
            return nums1[pos1 + k - 1];
        if (k == 1)
            return nums1[pos1] > nums2[pos2] ? nums2[pos2] : nums1[pos1];

        // update the position of the array
        int pos11 = (pos1 + k / 2 - 1) < (len1 -1) ? (pos1 + k / 2 - 1) : (len1 -1);
        int pos22 = (pos2 + k / 2 - 1) < (len2 -1) ? (pos2 + k / 2 - 1) : (len2 -1);
        if (nums1[pos11] <= nums2[pos22]) {
            k = k - (pos11 - pos1 + 1);
            pos1 = pos11 + 1;
        } else {
            k = k - (pos22 - pos2 + 1);
            pos2 = pos22 + 1;
        }
    }
}

double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
    int len1 = nums1.size();
    int len2 = nums2.size();
    int len = len1 + len2;
    if (len % 2 == 0) {
        int num1 = find_k_element(nums1, nums2, len / 2);
        int num2 = find_k_element(nums1, nums2, len / 2 + 1);
        return (num1 + num2) / 2.0;
    } else
        return find_k_element(nums1, nums2, len / 2 + 1);
    return 0;
}

int main() {
    std::vector<int> arr1;
    arr1.push_back(1);
    arr1.push_back(2);
    arr1.push_back(3);
    arr1.push_back(4);
    arr1.push_back(5);
    std::vector<int> arr2;
    arr2.push_back(6);
    arr2.push_back(7);
    arr2.push_back(8);
    arr2.push_back(9);
    arr2.push_back(10);
    std::cout << findMedianSortedArrays(arr1, arr2);
    return 0;
}