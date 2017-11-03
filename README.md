# darknet_cv
Данный репозиторий содержит скомпилированный Darknet Yolo с OpenCV и GPU.
Присутсвуют некоторые исправления и добавления в src файлах.
Требования к собственной обучающей выборке:
- должна содержать все ракурсы объекта;
- должна содержать некоторое колличество фотографий, где обьект расположен на разном расстоянии; 
- зоны интереса (ROI) должны быть заданы по следующим условиям:
1) не должно быть пересечения с другими искомыми объектами;
2) ROI не должна выходить за пределы объекта (проблемма с деформированным объектом);
- должны присутствовать негативные изображения, не содержащие объект.
