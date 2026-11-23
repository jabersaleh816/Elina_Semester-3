#include <iostream>
#include <vector>
#include "Algorithms.h"
#include "Course.h"

namespace {

long long sumEnrollments(const std::vector<Course>& courses) {
    long long total = 0;
    for (const Course& course : courses) {
        total += course.enrollments;
    }
    return total;
}

void printCourseListSection(const char* header,
                            const std::vector<Course>& selected) {
    std::cout << header << '\n';
    std::cout << selected.size() << ' '
              << sumEnrollments(selected) << '\n';
    for (const Course& course : selected) {
        std::cout << course.id << ' ' << course.start << ' '
                  << course.finish << ' ' << course.enrollments << '\n';
    }
}

} // namespace

int main() {
    int n = 0;
    if (!(std::cin >> n)) {
        return 0;
    }

    std::vector<Course> originalCourses;
    originalCourses.reserve(n > 0 ? static_cast<std::size_t>(n) : 0);
    for (int i = 0; i < n; ++i) {
        Course course;
        std::cin >> course.id >> course.start >> course.finish
                 >> course.enrollments;
        originalCourses.push_back(course);
    }

    std::vector<Course> sortedByFinish = originalCourses;
    mergeSort(sortedByFinish);

    std::vector<Course> countCourses = selectCourses(sortedByFinish);
    std::vector<std::vector<Course>> classrooms =
        allocateClassrooms(originalCourses);
    std::vector<Course> enrollmentCourses =
        selectCoursesDP(sortedByFinish);

    printCourseListSection("COUNT", countCourses);

    std::cout << "ROOMS\n";
    std::cout << classrooms.size() << '\n';
    for (std::size_t room = 0; room < classrooms.size(); ++room) {
        for (const Course& course : classrooms[room]) {
            std::cout << (room + 1) << ' ' << course.id << ' '
                      << course.start << ' ' << course.finish << ' '
                      << course.enrollments << '\n';
        }
    }

    printCourseListSection("ENROLLMENTS", enrollmentCourses);
    return 0;
}
