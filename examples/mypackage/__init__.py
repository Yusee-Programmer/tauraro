# Package initialization
print("Loading mypackage/__init__.py")

from .utils import helper_func
from .core import CoreClass

__version__ = "2.0.0"
__all__ = ["helper_func", "CoreClass"]
