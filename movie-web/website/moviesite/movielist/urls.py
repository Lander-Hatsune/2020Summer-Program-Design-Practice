from django.urls import path
from . import views

urlpatterns = [
    path(r'', views.index, name='index'),
    path(r'<int:page>/', views.index, name='index'),
    path(r"<str:title>/", views.info, name='info'),
]
